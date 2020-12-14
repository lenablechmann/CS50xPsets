from flask import Flask, render_template, request, redirect, session
from flask_session import Session

app = Flask(__name__)
# some magical configurations
app.config["SESSION_PERMANENT"] = False
# where we gonna store the data? in the filesystem of the server.
app.config["SESSION_TYPE"] = "filesystem"
# enabling sessions, enabling access to a python dictionary "session"
# which will have the users current interaction with the webpage,
# don't need todos[] anymore
Session(app)



@app.route("/")
def tasks():
    # using session dictionary keys instead of the ol todo dictionary
    # checks if there is a key "todos" inside the current session, if nah, creates a key + empty list
    if "todos" not in session:
        session["todos"] = []
    # show the current user's todo list from the session dictionary
    return render_template("tasks.html", todos = session["todos"])

# automatically only accepts the get method


@app.route("/add", methods=["GET", "POST"])
def add():
    if request.method == "GET":
        return render_template("add.html")
    else:
        # task is the input field, so todo is the user input
        todo = request.form.get("task")
        # appending it to the current user's session todo
        session["todos"].append(todo)
        return redirect("/")