//"com" is the variable that will contain the serial port name 
//that we will connect to.
//"serialPort" will be the variable that contains the instance of 
//the serialport object that we will create below.
var com;
var serialPort;

//This block of code is used to find what serial port the server
//should connect to. The "list" function that is part of the 
//"serialport" module lists all the available serial ports. Then,
//we loop over all the ports and find the one with the manufacturer 
//name that contains "arduino". Because we know this is the 
//correct serial port, we asign it to com.
var serialPorty = require("serialport");
serialPorty.list(function (err, ports) {
  ports.forEach(function(port) { 
    if(port.manufacturer.toLowerCase().indexOf("arduino") > -1) {
      com = port.comName;
      console.log(com);
    }
  });
});

//Here we import some of the necessary module that we will be using
//to build the app.
var express = require("express");
var app = express();
var httpServer = require("http").createServer(app);
var SerialPort = require("serialport").SerialPort;
var io = require("socket.io")(httpServer);
var port = 12345;

//Get the server to listen on port 12345.
httpServer.listen(port);

console.log("Server is available... Waiting for connection...");

//Have server send the index.html file to the client when they 
//connect to localhost:8080 (i.e. the root directory of the server).
app.get('/', function(req, res) {
  res.sendfile(__dirname + '/index.html')
});

//Create instance of the SerialPort object and assign it to serialPort.
//Note: The setTimeout function is necessary because of the asynchronous 
//nature of node.js (i.e. we can't really predict what order things will
//execute in. In this case, a timeout is necessary to allow things to happen 
//in the correct order).
setTimeout(function() {
  serialPort = new SerialPort(com, {baudrate: 9600});
}, 300);

//"io" (defined above) is listening for the "connection" event to occur.
//When the server connects to the client and the web socket connection 
//is established, execute a callback function that takes the socket object
//as input. 
//A callback function is simply a function that is only executed once a 
//specified event occurs (such as the connection event in this case).
io.on('connection', function(socket) {
  console.log("Client connected!");

  /************************************
  HANDLING DIRECTION CHANGES ONLY
  ************************************/

  //When the websocket receives the "message" event from the client,
  //it executes a callback function that takes as input the incoming 
  //data from the client.
  socket.on('message', function(data) {

    //Open up the serial port.
    //If there is no error, set the "speed" variable to be equal
    //to the incoming data.
    serialPort.open(function (error) {
      if(error) {
        console.log("Failed to open: " + error);
      } else {
        var speed = data;

        if (serialPort.isOpen()){
          console.log("Serial port open!")

          //Again, the asynchronous nature of node.js makes things
          //happen in strange orders. Therefore we need this setTimeout
          //function in order for everything to happen in the correct order.
          setTimeout(function() {
              //Write the speed variable to the serial port.
              serialPort.write(speed);              

              //These two lines aren't adding any functionality to the 
              //application. When the serialPort receives data, it will 
              //simply log it to the console.
              serialPort.on('data', function(data) {
               console.log('Data Received: ' + data);
            });
          }, 1600);
        }
      }
    });
  });

  /************************************
  HANDLING SPEED CHANGES ONLY
  ************************************/

  //When the websocket receives the "changeSpeed" event from the client,
  //it executes a callback function that takes as input the incoming 
  //data from the client.
  socket.on('changeSpeed', function(data) {

    //Open up the serial port.
    //If there is no error, set the "speed" variable to be equal
    //to the split incoming array of data.
    serialPort.open(function (error) {
      if(error) {
        console.log("Failed to open: " + error);
      } else {
        var speed = data.split("|");
        console.log(speed[0]);
        console.log(speed[1]);

        if (serialPort.isOpen()){
          console.log("Serial port open!")

          //Again, must use setTimeout function for things to 
          //happen in correct order.
          setTimeout(function() {

              //Write the first piece of data to the serial port.
              serialPort.write(speed[0]);

              //Write the second piece of data to the serial port.
              setTimeout(function() {
                serialPort.write(speed[1]);
              }, 1400); //Note this timeout could actually be
              //set as low as 1000. Just doing 1400 to be safe.
              
              serialPort.on('data', function(data) {
               // console.log('Data Received: ' + data);
            });
          }, 1600);
        }
      }
    });
  });
});






















