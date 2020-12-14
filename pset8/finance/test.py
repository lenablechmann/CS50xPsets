import os

from cs50 import SQL
from helpers import  lookup, usd

db = SQL("sqlite:///finance.db")

portfolio = db.execute("SELECT symbol, shares_current FROM portfolio WHERE user_id=4;")

if not portfolio:
    print("rip")

# Price total for all the shares
total_stocks = 0.0

for row in range(len(portfolio)):
    # looks up the current price of the stock in question
    result_lookup = lookup(portfolio[row]["symbol"])

    # inserts the data into the dict
    portfolio[row]["name"] = str(result_lookup["name"])
    portfolio[row]["price"] = int(result_lookup["price"])

    # computes the total price of the shares the user has
    price_shares = portfolio[row]["price"] * portfolio[row]["shares_current"]
    portfolio[row]["price_shares"] = price_shares

    # calculating the price of all stocks over the for loop
    total_stocks += price_shares

print(portfolio)