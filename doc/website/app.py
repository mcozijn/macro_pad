from flask import Flask, render_template, request, redirect, url_for
from flask_socketio import SocketIO, emit
from gevent import monkey

app = Flask(__name__)
socketio = SocketIO(app, async_mode='gevent')

LINUX_USERS = 0
WINDOWS_USERS = 0
MAC_USERS = 0

@app.route('/')
def index():
    return render_template('index.html', linux_users=LINUX_USERS, windows_users=WINDOWS_USERS, mac_users=MAC_USERS)

@app.route('/linux')
def linux():
    return render_template('linux.html', linux_users=LINUX_USERS, windows_users=WINDOWS_USERS, mac_users=MAC_USERS)

@app.route('/windows')
def windows():
    return render_template('windows.html', linux_users=LINUX_USERS, windows_users=WINDOWS_USERS, mac_users=MAC_USERS)

@app.route('/select_os', methods=['POST'])
def select_os():
    global LINUX_USERS, WINDOWS_USERS, MAC_USERS
    selected_os = request.form.get('os')

    if selected_os == 'linux':
        LINUX_USERS += 1
    elif selected_os == 'mac':
        MAC_USERS += 1
    elif selected_os == 'windows':
        WINDOWS_USERS += 1

    counts = {
        'linux_users': LINUX_USERS,
        'mac_users': MAC_USERS,
        'windows_users': WINDOWS_USERS
    }
    socketio.emit('update_counts', counts)

    if selected_os == 'linux' or selected_os == 'mac':
        return redirect(url_for('linux'))
    elif selected_os == 'windows':
        return redirect(url_for('windows'))
    else:
        return redirect(url_for('index'))

@app.route('/os_counts')
def os_counts():
    return render_template('os_count.html')

@socketio.on('request_counts')
def handle_request_counts():
    counts = {
        'linux_users': LINUX_USERS,
        'mac_users': MAC_USERS,
        'windows_users': WINDOWS_USERS
    }
    emit('initial_counts', counts)

if __name__ == '__main__':
    socketio.run(app, debug=True)
