var net = require('net');

var clientCount = 0;

var clients = [];

var server_out = net.createServer(function (socket) {
  socket.write("Echo server");
  socket.id = clientCount;
  socket.pipe(socket);
  
  if(!clients[socket.id]) clients[socket.id] = {};
  clients[socket.id].socket_in = socket;

  var disconnected = false;
  setTimeout(function tim() {
    if(disconnected) return;
    var txt = "Still alive for client " + socket.id + " timestamp: " + Date.now();
    socket.write(txt);
    console.log(txt);
    setTimeout(tim, 1000);
  }, 1000);

    socket.on('end', function() {
      disconnected = true;
      console.log('client out disconnected');
    });

});
server_out.listen(1337, "127.0.0.1");

var server_in = net.createServer(function (socket) {
  socket.write("Echo server");
  socket.id = clientCount;
  socket.pipe(socket);
  
  if(!clients[socket.id]) clients[socket.id] = {};
  clients[socket.id].socket_out = socket;

  var disconnected = false;

  socket.on('data', function(data) {
      console.log(data.toString());
      if(!clients[socket.id].socket_in) {
	  console.log("socket not ready yet");
	  return;
      }
      clients[socket.id].socket_in.write("server got data: " + data.toString());
      socket.end();
    });
    socket.on('end', function() {
      disconnected = true;
      console.log('client in disconnected');
    });

});
server_in.listen(1338, "127.0.0.1");

console.log('Server running at http://127.0.0.1:1337/');
