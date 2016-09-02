const express = require('express');
const bodyParser = require('body-parser')
const MongoClient = require('mongodb').MongoClient

const app = express();
var db;

app.use(bodyParser.urlencoded({extended: true}));
app.use(express.static(__dirname + '/public'));
app.set('view engine', 'ejs');

//app.listen(3000, function() {
//	console.log('listening on 3000');
//});

app.get('/', (req, res) => {
	// res.send('The chillest of wines.');
	//var cursor = db.collection('quotes').find();
//	db.collection('quotes').find().toArray(function(err, result) {
//		console.log(result);
	//res.sendFile(__dirname + '/views/store.html');
//		res.render('index.ejs', {quotes: result})
//	});
	res.render('store.ejs');
});


app.get('/place', (req, res) => {
	// res.send('The chillest of wines.');
	//var cursor = db.collection('quotes').find();
//	db.collection('quotes').find().toArray(function(err, result) {
//		console.log(result);
	//res.sendFile(__dirname + '/views/store.html');
//		res.render('index.ejs', {quotes: result})
//	});
	res.render('place.ejs');
});

app.get('/remove', (req, res) => {
	// res.send('The chillest of wines.');
	//var cursor = db.collection('quotes').find();
//	db.collection('quotes').find().toArray(function(err, result) {
//		console.log(result);
	//res.sendFile(__dirname + '/views/store.html');
//		res.render('index.ejs', {quotes: result})
//	});
	res.render('remove.ejs');
});


//app.post('/quotes', (req, res) => {
//	db.collection('quotes').save(req.body, (err, result) => {
//		if (err) return console.log(err);
//		
//		console.log('saved to database');
//		res.redirect('/');
//	});
//});


MongoClient.connect('mongodb://localhost:27017/winechills', (err, database) => {
	// ...	start the server
	if (err)  return console.log(err);
	db = database;
	app.listen(3000, () => {
		console.log('listening on 3000');
	});
});

console.log('MayNode be with you');
