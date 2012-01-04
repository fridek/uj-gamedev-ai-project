var net = require('net');

var clientCount = 0;

var server = net.createServer(function (socket) {
  socket.write("Echo server");
  socket.id = clientCount++;
  socket.pipe(socket);

  var disconnected = false;
  setTimeout(function tim() {
    if(disconnected) return;
    var txt = "Still alive for client " + socket.id + " timestamp: " + Date.now();
    socket.write(txt);
    console.log(txt);
    setTimeout(tim, 1000);
  }, 1000);

//  console.log(socket);
    socket.on('data', function(data) {
      console.log(data.toString());
      socket.write("server got data: " + data.toString());
      socket.end();
    });
    socket.on('end', function() {
      disconnected = true;
      console.log('client disconnected');
    });

});
server.listen(1337, "127.0.0.1");

console.log('Server running at http://127.0.0.1:1337/');
