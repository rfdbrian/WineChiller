var serial = require('serialport');

var port = new serial('/dev/ttyACM0', {
	baudRate: 9600,
	parser: serial.parsers.readline('\n')
});

var simPort = new serial('/dev/ttyUSB0', {
	baudRate: 9600,
	parser: serial.parsers.readline('\n'),
	autoOpen: true
});

port.on('error', function(err) {
	console.log('Error: ', err.message);
});

simPort.on('error', function(err) {
	console.log('Error: ', err.message);
});

port.on('data', function(data) {
	var toSend = String.fromCharCode(data.charCodeAt(0) + 17) + data.slice(1, 2) + ':' + data.slice(2,3);
	if (data.slice(0,5) === '00001') {
		toSend = 'NA';
		console.log('this is...');
	};
	switch (toSend.slice(0,2)) {
		case "A0":
			toSend = "4E:" + data.slice(2,3);
			break;

		case "A1":
			toSend = "2E:" + data.slice(2,3);
			break;

		case "A2":
			toSend = "4D:" + data.slice(2,3);
			break;

		case "B0":
			toSend = "3E:" + data.slice(2,3);
			break;

		case "B1":
			toSend = "3D:" + data.slice(2,3);
			break;

		case "B2":
			toSend = "2D:" + data.slice(2,3);
			break;
		
		default:
			toSend = "";
			break;
	}

	if (toSend.length == 4) {
		console.log(toSend);
		simPort.write(toSend, (error) => {
			if (error) {
				return console.log('Error: ', error.message);
			}
			console.log('message written');
		});	 
	};
});
