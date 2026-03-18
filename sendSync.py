import socketio
import time
import sys 
import re
import os
import threading

# standard Python
sio = socketio.Client(logger=False, engineio_logger=False)
_ack = threading.Event()

@sio.event
def connect():
        print("I'm connected!")
        #sio.emit('message', 'Hello world\n')

@sio.event
def connect_error(data):
        print("Connection failed:", data)

@sio.on('handshake')
def on_message(data):
        print('HandShake', data)

@sio.event
def message(data):
        print('I received a message!')

#sio.connect('http://localhost:4040')
server_url = os.environ.get("SYNC_SERVER_URL", "http://47.131.91.151:4040")
sio.connect(server_url)
# sio.connect(sys.argv[1])


#workspaceurl = re.sub("@[0-9]","", sys.argv[5])

sendMsg = {"BuildID":sys.argv[2], "Branch":sys.argv[3], "CommitID":sys.argv[4] , "Workspace":sys.argv[5], "Module":sys.argv[6]}

time.sleep(1)
print("Emitting message to", server_url, "payload:", sendMsg)

def _on_ack(*args):
        print("Server ACK:", args if args else "(empty)")
        _ack.set()

sio.emit('message', sendMsg, callback=_on_ack)
time.sleep(1.5)
if not _ack.is_set():
        print("No ACK received within timeout (server may not send ack).")

try:
        sio.disconnect()
except Exception as e:
        print("Disconnect error:", e)
