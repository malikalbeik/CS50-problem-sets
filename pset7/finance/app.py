from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    return apology("TODO")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    if request.method == "POST":
        # ensure shares symbol was submitted
        if not request.form.get("share"):
            return apology("must provide share symbol")

        # ensure the share's symbol is valid was submitted
        elif not lookup(request.form.get("share")):
            return apology("must provide a valid share symbol")

        # ensure the number of shares to be bought was submitted
        elif not request.form.get("number"):
            return apology("must provide a number of shares to be bought symbol")

        # ensure the number of shares to be bought is positive submitted 
        elif int(request.form.get("number")) < 0:
            return apology("must provide a positive number of shares to be bought symbol")

        shares = request.form.get("number")
        stock = lookup(request.form.get("share"))
        cash = db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

        if cash < int(request.form.get("number")) * stock['price']:
            return apology("You don't have enough money to buy this stock")
        else:
            remainingCash = float(cash) - float(shares) * float(stock['price'])
            db.execute("UPDATE users SET cash = :remainingCash WHERE id = :id", id=session["user_id"], remainingCash=remainingCash)
            db.execute("INSERT INTO purchases (user_id, price_bought, stock_symbol, shares) \
                             VALUES(:user_id, :price_bought, :stock_symbol, :shares)", \
                             user_id = session["user_id"], \
                             price_bought = stock['price'], \
                             stock_symbol = stock['symbol'], \
                             shares = shares)
                             

            return render_template("bought.html", shares=shares, price=stock['price'], symbol=stock['symbol'], remainingCash= round(db.execute("SELECT cash FROM users WHERE id = :id", id=session["user_id"])[0]["cash"], 2))
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    return apology("TODO")

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        rows = lookup(request.form.get("quote"))
        if not request.form.get("quote"):
            return apology("you must provide a stock name")
        elif not rows:
            return apology("there is no stock with that name")
        else:
            return render_template("quoted.html",stock = rows)
    else:
        return render_template("quote.html")




@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    if request.method == "POST":
        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # ensure retyped password was submitted
        elif not request.form.get("password2"):
            return apology("must retype your password")

        # ensure retyped password was submitted
        elif request.form.get("password2") != request.form.get("password"):
            return apology("the passwords that you provided must be the same")

        result = db.execute("INSERT INTO users (username, hash) \
                             VALUES(:username, :hash)", \
                             username = request.form.get("username"), \
                             hash = pwd_context.hash(request.form.get("password", str)))

        if not result:
            return apology("Username already exist")

        # remember which user has logged in
        session["user_id"] = result

        # redirect user to home page
        return redirect(url_for("index"))

    else:
        return render_template("register.html")

    return apology("TODO")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    return apology("TODO")

