var express = require('express')
var app = express()
var fs = require('fs');
mysql = require('mysql');
var connection = mysql.createConnection({
    host: 'localhost',
    user: 'sensor',
    password: 'mypassword',
    database: 'data'
})
connection.connect();

app.get('/', function (req, res) {
  console.log(req.query);

  var qstr = 'select * from sensors where device = 100 limit 4000';
  connection.query(qstr, function(err, rows, cols) {
    if (err) {
      throw err;
      res.send('query error: '+ qstr);
      return;
    }

    console.log("Got "+ rows.length +" records");
    var html = "['Time', 'PM2.5', 'PM10']"
    for (var i=0; i< rows.length; i++) {
       var pm25 = "null";
       var pm10 = "null";
       if (rows[i].unit == 0) pm25 = rows[i].value;
       else pm10 = rows[i].value;
       html += ",["+ i +","+ pm25 +","+ pm10 +"]";
    }

    fs.readFile("graph.html", 'utf8', function(err, data) {
      if (err) conosole.log("file error" + err);

      data = data.replace("<%DATADATA%>", html);
      res.send(data);
    });
  });

})

app.listen(3000, function () {
  console.log('Example app listening on port 3000!')
})
