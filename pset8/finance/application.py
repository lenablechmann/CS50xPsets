import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
import datetime

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    if request.method == "GET":
        # For loop to write the data into the dictionary, which we will later iterate over in jinja
        portfolio = db.execute("SELECT symbol, shares_current FROM portfolio WHERE user_id=:user_id ORDER BY symbol;", user_id = session["user_id"])

        if not portfolio:
            return apology("You don't have any shares atm.", 403)

        else:
            # Price total for all the shares
            total_stocks = 0.0

            # didn't work but inrange solved it
            # https://stackoverflow.com/questions/37381747/list-indices-must-be-integers-or-slices-not-dict
            for row in range(len(portfolio)):

                # looks up the current price of the stock in question
                result_lookup = lookup(portfolio[row]["symbol"])

                # inserts the data into the dict
                portfolio[row]["name"] = result_lookup["name"]
                # round(Decimal(0.3223322), 2)
                portfolio[row]["price"] = result_lookup["price"]

                # computes the total price of the shares the user has
                price_shares = portfolio[row]["price"] * portfolio[row]["shares_current"]
                portfolio[row]["price_shares"] = usd(price_shares)
                # converting it into usd after using it for calculations
                portfolio[row]["price"] = usd(portfolio[row]["price"])

                # calculating the price of all stocks over the for loop
                total_stocks += price_shares

            # lookup user total cash with db.execute
            cash = db.execute("SELECT cash FROM users WHERE id=:id;", id = session["user_id"])
            total_cash = round((cash[0]["cash"]), 2)
            total_value = usd(total_cash + total_stocks)
            total_cash = usd(total_cash)

            return render_template("index.html", portfolio = portfolio, total_cash = total_cash, total_value = total_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")

    elif request.method == "POST":
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        # stores the number of shares in an int
        shares = int(request.form.get("shares"))

        if shares < 0:
            return apology("shares must be a positive number", 403)
        # looks up the symbol with the helpers.py function. result is a dictionary
        result_lookup = lookup(request.form.get("symbol"))
        # checks if symbol actually exists
        if result_lookup == None:
            return apology("stock symbol doesn't exist", 403)

        # prepares the data for the html
        stock_name = result_lookup["name"]
        symbol = result_lookup["symbol"]
        price = result_lookup["price"]

        # checking the db if user has enough cash for the transaction
        rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id = session["user_id"])
        if (price * shares) > rows[0]["cash"]:
            return apology("you don't have enough cash", 403)

        # buy the stocks for the current price and update the database
        elif (price * shares) <= rows[0]["cash"]:
            # calculating how much cash will be over after the transaction:
            total = rows[0]["cash"] - (price * shares)

            # updating cash
            db.execute("UPDATE users SET cash = :cash WHERE id = :id",
                          cash = total, id = session["user_id"])

            # updating the history table
            db.execute("INSERT INTO history (user_id, symbol, stock_name, shares, date, price, total) VALUES (:user_id, :symbol, :stock_name, :shares, strftime('%s','now'), :price, :total)",
                        user_id = session["user_id"], symbol = symbol, stock_name = stock_name, shares = shares, price = price, total = (price*shares))

            # checking if user has shares of this stock already
            current = db.execute("SELECT shares_current FROM portfolio WHERE user_id = :user_id AND symbol = :symbol",
                          user_id = session["user_id"], symbol = symbol)
            # If user has none of that stock yet, adds a new row into the table
            if not current:
                db.execute("INSERT INTO portfolio (user_id, symbol, shares_current) VALUES (:user_id, :symbol, :shares_current)",
                        user_id = session["user_id"], symbol = symbol, shares_current = shares)

            # Updates the portfolio if the user has some shares of that stock (adds to the existing shares)
            else:
                shares_current = shares + current[0]["shares_current"]
                db.execute("UPDATE portfolio SET shares_current = :shares_current WHERE user_id = :user_id AND symbol = :symbol)",
                        shares_current = shares_current, user_id = session["user_id"], symbol = symbol)
    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    # copy from the index
    if request.method == "GET":
        # For loop to write the data into the dictionary, which we will later iterate over in jinja
        history = db.execute("SELECT symbol, stock_name, shares, date, price, total FROM history WHERE user_id=:user_id ORDER BY date DESC;", user_id = session["user_id"])

        if not history:
            return apology("You haven't transacted anything yet.", 403)

        else:
            # Price total for all the shares
            total_stocks = 0.0

            # didn't work but inrange solved it
            # https://stackoverflow.com/questions/37381747/list-indices-must-be-integers-or-slices-not-dict
            for row in range(len(history)):
                # converts the unix time from the table into datetime format
                time = int(history[row]["date"])
                timestamp = datetime.datetime.fromtimestamp(time)
                history[row]["date"] = str(timestamp.strftime('%Y-%m-%d %H:%M:%S'))

            return render_template("history.html", history = history)



@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    # make a form to request stock quote (GET)
    # testing the dictionary via lookup("AAPL") gave {'name': 'Apple, Inc.', 'price': 119.03, 'symbol': 'AAPL'}
    if request.method == "GET":
        return render_template("quote.html")

    elif request.method == "POST":
        # looks up the symbol with the helpers.py function. result is a dictionary
        result_lookup = lookup(request.form.get("symbol"))

        # checks if symbol actually exists
        if result_lookup == None:
            return apology("stock symbol doesn't exist", 403)

        # prepares the data for the html
        name = result_lookup["name"]
        symbol = result_lookup["symbol"]
        price = result_lookup["price"]

        # returns a special html template
        return render_template("quoted.html", name=name, symbol=symbol, price=price)


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    if request.method == "GET":
        return render_template("register.html")

    elif request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password") or not request.form.get("password2"):
            return apology("must provide password", 403)

        elif request.form.get("password") != request.form.get("password2"):
            return apology("reenter your password to confirm it", 403)

        # saving password in a string, checking if long enough:
        pw = str(request.form.get("password"))
        if len(pw) < 5:
            return apology("your password is too short", 403)

        # If username is already taken
        rows = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
        if len(rows) >= 1:
            return apology("Username already taken", 403)

        # hashing the password
        hash = generate_password_hash(pw)

        # storing username and hash in the database
        db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                    username = request.form.get("username"), hash = hash)
        # make em log in automatically and send em into the index.html
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))
        session["user_id"] = rows[0]["id"]

        return redirect("/")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    # repurpose buy
    if request.method == "GET":
        return render_template("sell.html")

    elif request.method == "POST":

        # basic error checking
        if not request.form.get("symbol"):
            return apology("must provide symbol", 403)
        elif not request.form.get("shares"):
            return apology("must provide shares", 403)

        # stores the number of shares in an int
        shares = int(request.form.get("shares"))

        if shares < 0:
            return apology("shares must be a positive number", 403)
        # looks up the symbol with the helpers.py function. result is a dictionary
        result_lookup = lookup(request.form.get("symbol"))

        # checks if symbol actually exists
        if result_lookup == None:
            return apology("stock symbol doesn't exist", 403)

        # prepares the data for the html
        symbol = result_lookup["symbol"]
        price = result_lookup["price"]
        stock_name = result_lookup["name"]

        # checks if the user actually owns the stock mentioned and has enough shares to sell:
        current = db.execute("SELECT shares_current FROM portfolio WHERE user_id = :user_id AND symbol = :symbol",
                          user_id = session["user_id"], symbol = symbol)
        if not current:
            return apology("you don't own any shares of that stock", 403)
        if current[0]["shares_current"] < shares:
            return apology("you don't own enough shares", 403)

        # sells the stocks for the current price and update the database
        else:
            # updating the history table
            db.execute("INSERT INTO history (user_id, symbol, stock_name, shares, date, price, total) VALUES (:user_id, :symbol, :stock_name, :shares, strftime('%s','now'), :price, :total)",
                        user_id = session["user_id"], symbol = symbol, stock_name = stock_name, shares = (shares*(-1)), price = price, total = (price*shares))

            # updates portfolio
            # if current shares > shares, then just substract and update portfolio
            if current[0]["shares_current"] > shares:

                shares_current = current[0]["shares_current"] - shares

                db.execute("UPDATE portfolio SET shares_current = :shares_current WHERE user_id = :user_id AND symbol = :symbol",
                        shares_current = shares_current, user_id = session["user_id"], symbol = symbol)

            # if they are equal, delete row from table
            elif current[0]["shares_current"] == shares:
                db.execute("DELETE FROM portfolio WHERE user_id = :user_id AND symbol = :symbol", user_id = session["user_id"], symbol = symbol)

            # calculate new cash total
            rows = db.execute("SELECT cash FROM users WHERE id = :id",
                          id = session["user_id"])
            cash = rows[0]["cash"]
            sold = price * shares
            cash += sold
            cash = round((cash), 2)

            # update the cash in the users table
            db.execute("UPDATE users SET cash = :cash WHERE id = :id", cash=cash, id = session["user_id"])

            return redirect("/")



def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
