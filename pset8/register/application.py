from cs50 import SQL
from flask import Flask, render_template, request, redirect, session
from flask_session import Session

app = Flask(__name__)

# adding our db
db = SQL("sqlite:///lecture.db")

@app.route("/")
def index():
    # storing results of the query in a variable:
    rows = db.execute("SELECT * FROM registrants;")
    # passing the variable rows into a template, so it can be rendered on the website
    return render_template("index.html", rows = rows)

# Registration
@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "GET":
        return render_template("register.html")
    else:
        # post method
        name = request.form.get("name")
        if not name:
            return render_template("apology.html", message="You must provide a name.")
        email = request.form.get("email")
        if not email:
            return render_template("apology.html", message="You must provide an email address.")
        # inserting into database while avoiding SQL injection
        db.execute("INSERT INTO registrants (name, email) VALUES (:name, :email);", name=name, email=email)
        return redirect("/")