var net = require('net');

var clientCount = 0;

var clients = [];

var map = [
  {
    type: 'rectangle',
    vertices: [[14,6],[17,8],[13,12],[10,10]]
  },
  {
    type: 'rectangle',
    vertices: [[314,106],[317,108],[313,112],[310,110]]
  },
  {
    type: 'rectangle',
    vertices: [[500,530],[530,500],[560,530],[530,560]]
  },
  {
    type: 'rectangle',
    vertices: [[200,430],[230,400],[260,430],[230,460]]
  }
];

function setMapToClient(id) {
  if(!clients[id].socket_in) {
    console.log("client not ready, retry");
    setTimeout(function() {setMapToClient(id);}, 50);
    return;
  }
  var data = {
    type: "init_map",
    map_size: [800,600],
    map: map,
    position: [200,300],
    ammo: [100,10],
    current_timestamp: Date.now(),
    start_timestamp: Date.now()
  };
  console.log("sending map to client " + id, data);
  clients[id].socket_in.write(JSON.stringify(data) + '\0');
};


var server_out = net.createServer(function (socket) {
  socket.write("Echo server");
  socket.id = clientCount;
  socket.pipe(socket);
  
  if(!clients[socket.id]) clients[socket.id] = {};
  else clientCount++;

  clients[socket.id].socket_in = socket;
  
  setMapToClient(socket.id);

  var disconnected = false;
  setTimeout(function tim() {
    if(disconnected) return;
    var heartbeat = {
	type: "heartbeat",
	player_id: socket.id,
	timestamp: Date.now()
    };
    socket.write(JSON.stringify(heartbeat) + '\0');
    console.log(heartbeat);
    setTimeout(tim, 1000);
  }, 1000);

    socket.on('end', function() {
      disconnected = true;
      clients[socket.id] = false;
      console.log('client out disconnected');
    });

});
server_out.listen(1337, "127.0.0.1");

var server_in = net.createServer(function (socket) {
  socket.write("Echo server");
  socket.id = clientCount;
  socket.pipe(socket);
  
  if(!clients[socket.id]) clients[socket.id] = {};
  else clientCount++;
				 
  clients[socket.id].socket_out = socket;

  var disconnected = false;

  socket.on('data', function(data) {
      console.log(data.toString());
      if(!clients[socket.id].socket_in || disconnected) {
	  console.log("socket not ready");
	  return;
      }
      try {
	var b = JSON.parse(data);
	b.player_id = socket.id;
	for(var i = 0; i<clients.length;i++) {
	  if(socket.id != i && clients[i]) clients[i].socket_in.write(JSON.stringify(b) + '\0');
	}
      } catch(e) {
	console.log("some error with this message");
      }
    });
    socket.on('end', function() {
      disconnected = true;
      clients[socket.id] = false;
      console.log('client in disconnected');
    });

});
server_in.listen(1338, "127.0.0.1");

console.log('Server running at http://127.0.0.1:1337/');
