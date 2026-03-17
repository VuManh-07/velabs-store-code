import socketio
import time
import sys 
import re

# standard Python
sio = socketio.Client()

@sio.event
def connect():
        print("I'm connected!")
        #sio.emit('message', 'Hello world\n')

@sio.on('handshake')
def on_message(data):
        print('HandShake', data)

@sio.event
def message(data):
        print('I received a message!')

#sio.connect('http://localhost:4040')
#sio.connect('http://10.144.82.108:4040')
sio.connect(sys.argv[1])


#workspaceurl = re.sub("@[0-9]","", sys.argv[5])

sendMsg = {"BuildID":sys.argv[2], "Branch":sys.argv[3], "CommitID":sys.argv[4] , "Workspace":sys.argv[5], "Module":sys.argv[6]}

time.sleep(1)
sio.emit('message', sendMsg)
time.sleep(1.5)
