# Flask app
from flask import Flask, render_template

app = Flask(__name__)

LINUX_USERS = 0
WINDOWS_USERS = 0

@app.route('/')
def index():
    return "How did you get here?"

@app.route('/linux')
def linux():
    global LINUX_USERS
    return render_template('linux.html', linux_users=LINUX_USERS)
    LINUX_USERS += 1

@app.route('/windows')
def windows():
    global WINDOWS_USERS
    return render_template('windows.html', windows_users=WINDOWS_USERS)
    WINDOWS_USERS += 1