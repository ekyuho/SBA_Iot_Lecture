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

  a = {};
  a.seq = r.seq;
  a.device = r.device;
  a.unit = 0;
  a.type = r.type;
  a.value = r.pm25;
  a.ip = r.ip;

  var query = connection.query('insert into sensors set ?', a, function(err, rows, cols) {
    if (err) throw err;
    console.log("insert pm2.5");

    a = {};
    a.seq = r.seq;
    a.device = r.device;
    a.unit = 1;
    a.type = r.type;
    a.value = r.pm10;
    a.ip = r.ip;

    var query = connection.query('insert into sensors set ?', a, function(err, rows, cols) {
      if (err) throw err;

      res.send('OK: ' + JSON.stringify(r));
    });
  });
});

var server = app.listen(8080, function () {
  var host = server.address().address
  var port = server.address().port
  console.log('listening at http://%s:%s', host, port)
});
