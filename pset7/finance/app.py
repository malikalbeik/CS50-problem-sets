import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash
import time

from helpers import apology, login_required, lookup, usd

# Ensure environment variable is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")

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


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    purchases = db.execute("SELECT * FROM purchases WHERE user_id=:id AND sold=0", id=session["user_id"])
    if not purchases:
        return apology("you don't have any purchases to display", 200)

    else:
        total = 0
        for item in purchases:
            item["current_price"] = lookup(item["stock_symbol"])['price']
            total += item["current_price"]*item["shares"]
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
        return render_template("index.html", history=purchases, total=total, cash=cash)



@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":
        # ensure shares symbol was submitted
        if not request.form.get("symbol"):
            return apology("must provide share symbol")

        # ensure the share's symbol is valid was submitted
        elif not lookup(request.form.get("symbol")):
            return apology("must provide a valid share symbol")

        # ensure the number of shares to be bought was submitted
        elif not request.form.get("shares") or not request.form.get("shares").isdigit() :
            return apology("must provide a valid number of shares to be bought")

        # ensure the number of shares to be bought is positive submitted 
        elif int(request.form.get("shares")) < 0:
            return apology("must provide a positive number of shares to be bought symbol")

        shares = request.form.get("shares")
        stock = lookup(request.form.get("symbol"))
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

        if cash < int(request.form.get("shares")) * stock['price']:
            return apology("You don't have enough money to buy this stock")
        else:
            remainingCash = float(cash) - float(shares) * float(stock['price'])
            db.execute("UPDATE users SET cash = :remainingCash WHERE id = :id", id=session["user_id"], remainingCash=remainingCash)
            db.execute("INSERT INTO purchases (user_id, price_bought, stock_symbol, shares, sold) \
                             VALUES(:user_id, :price_bought, :stock_symbol, :shares, :sold)", \
                             user_id = session["user_id"], \
                             price_bought = stock['price'], \
                             stock_symbol = stock['symbol'], \
                             shares = shares, \
                             sold = 0)
            db.execute("INSERT INTO history (user_id, price, stock_symbol, shares, sold, time) \
                        VALUES(:user_id, :price, :stock_symbol, :shares, :sold, :time)", \
                        user_id = session["user_id"], \
                        price = stock['price'], \
                        stock_symbol = stock['symbol'], \
                        shares = shares, \
                        sold = 0, \
                        time = str(time.strftime("%d/%m/%Y-%H:%M:%S")))
                             

            return render_template("bought.html", total = int(shares)*int(stock['price']) , shares=shares, price=stock['price'], symbol=stock['symbol'], remainingCash= round(db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"], 2))
    else:
        return render_template("buy.html")



@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT * FROM history WHERE user_id=:id", id=session["user_id"])
    return render_template("history.html", history=history)


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
    if request.method == "POST":
        rows = lookup(request.form.get("symbol"))
        if not request.form.get("symbol"):
            return apology("you must provide a stock symbol")
        elif not rows:
            return apology("there is no stock with that symbol")
        else:
            return render_template("quoted.html",stock = rows)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # ensure retyped password was submitted
        elif not request.form.get("confirmation"):
            return apology("must retype your password", 400)

        # ensure retyped password was submitted
        elif request.form.get("confirmation") != request.form.get("password"):
            return apology("the passwords that you provided must be the same", 400)

        result = db.execute("INSERT INTO users (username, hash) \
                            VALUES(:username, :hash)", \
                            username = request.form.get("username"), \
                            hash = generate_password_hash(request.form.get("password"), method='pbkdf2:sha256', salt_length=8))

        if not result:
            return apology("Username already exist", 400)

        # remember which user has logged in
        session["user_id"] = result

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("register.html")



@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        sharesNum = int(request.form.get("shares"))
        stockSymbol = request.form.get("symbol")

        if not stockSymbol:
            return apology("please provide a stock to sell")

        if not sharesNum and sharesNum < 0:
            return apology("please provide a valid shares number")        

        # if sharesNum == db.execute("SELECT shares FROM purchases WHERE id=:id", id=request.form.get("stock")):
        #     return apology("you don't have enough shares")


        stocks = db.execute("SELECT * FROM purchases WHERE stock_symbol=:stock_symbol", stock_symbol = stockSymbol)
        stock_sold = None

        index = 0
        for stock in stocks:
            if stock["shares"] == sharesNum:
                db.execute
                db.execute("UPDATE purchases SET sold=1 WHERE id=:id", id=stock["id"])
                stock_sold = stock
                break
            elif stock["shares"] > sharesNum:
                db.execute("UPDATE purchases SET shares=:shares WHERE id=:id", shares = stock["shares"] - sharesNum, id = stock["id"])
                stock_sold = stock
                break

        if stock_sold:
            priceSold = lookup(stock_sold['stock_symbol'])['price']
            usersCash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]
            db.execute("UPDATE users SET cash=:cash WHERE id=:id", cash=usersCash+priceSold*stock['shares'], id=session["user_id"])

            db.execute("INSERT INTO history (user_id, price, stock_symbol, shares, sold, price_sold, time) \
                        VALUES(:user_id, :price, :stock_symbol, :shares, :sold, :price_sold, :time)", \
                        user_id = session["user_id"], \
                        price = stock_sold['price_bought'], \
                        stock_symbol = stock_sold['stock_symbol'], \
                        shares = sharesNum, \
                        sold = 1, \
                        price_sold = priceSold, \
                        time = str(time.strftime("%d/%m/%Y-%H:%M:%S")))

            # return render_template("sold.html", priceSold = priceSold, sharesNum=sharesNum, item=stock_sold, cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"])
            return redirect(url_for("index"))

        else:
            return apology("couldn't sell")

    else:
        items = db.execute("SELECT * FROM purchases WHERE sold=0")
        if not items:
            return apology("you don't have a stock to sell")
        return render_template("sell.html", items=items)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


@app.route("/profile", methods=["GET", "POST"])
@login_required
def profile():
    """Show the users profile and change name or password."""
    if request.method == "GET":
        userName = db.execute("SELECT username FROM users WHERE id=:id", id=session["user_id"])[0]['username']
        return render_template("profile.html", username=userName)
    
    else:
        # ensure password was submitted
        if not request.form.get("newpassword"):
            return apology("must provide a new password")

        # ensure retyped password was submitted
        elif not request.form.get("newpassword2"):
            return apology("must retype your old password")

        # ensure retyped password was submitted
        elif not request.form.get("oldpassword"):
            return apology("must provide your old password")

        # ensure new passwords match
        elif request.form.get("newpassword") != request.form.get("newpassword2"):
            return apology("the passwords that you provided must be the same")

        row = db.execute("SELECT * FROM users WHERE id=:id", id=session["user_id"])
        # ensure username exists and password is correct
        if not row or not pwd_context.verify(str(request.form.get("oldpassword")), row[0]['hash']):
            return apology("invalid password")

        db.execute("UPDATE users SET hash=:hash WHERE id=:id", id=session["user_id"], hash=pwd_context.hash(request.form.get("newpassword", str))) 

        # redirect user to home page
        return redirect(url_for("index"))