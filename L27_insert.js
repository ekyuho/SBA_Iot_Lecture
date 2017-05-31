var express = require('express');
var app = express();

mysql = require('mysql');
var connection = mysql.createConnection({
    host: 'localhost',
    user: 'sensor',
    password: 'mypassword',
    database: 'data'
})
connection.connect();

app.get('/logone', function(req, res) {
  r = req.query;
  console.log("GET %j", r);

  var query = connection.query('insert into sensors set ?', r, function(err, rows, cols) {
    if (err) throw err;
      console.log("done");
    });    
  });

var server = app.listen(8080, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('listening at http://%s:%s', host, port)
});
