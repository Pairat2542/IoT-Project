import 'dart:async';

import 'package:flutter/material.dart';
import 'dart:convert';
import 'package:http/http.dart' as http;

import 'dart:io';
import 'thirdscreen.dart';

class SecondScreen extends StatelessWidget {
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: MyHomePage(),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key key}) : super(key: key);

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  @override
  void initState() {
    super.initState();

    _startTimer();
  }

  Timer _timer;

  void _startTimer() {
    _timer = Timer.periodic(Duration(seconds: 3), (timer) {
      setState(() {
        http.get('http://192.168.43.145:8080/').then((response) {
          if (response.statusCode == 200) {
            var resJson = json.decode(response.body);
            stdout.writeln(resJson);
            if (resJson["status"] == true && resJson["state"] == 2) {
              timer.cancel();
              Navigator.pushReplacement(context,
                  MaterialPageRoute(builder: (context) => Thirdscreen()));
            }
          } else {
            throw Exception('Failed to load album');
          }
        });
      });
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: Text("ROVER NETWORK PROJECT"),
        ),
        body: SafeArea(
          child: Stack(
            children: <Widget>[
              Container(
                  decoration: BoxDecoration(
                    image: DecorationImage(
                      image: AssetImage("image/ui2.jpg"),
                      fit: BoxFit.cover,
                    ),
                  ),
                  child: null),
              Padding(
                padding: EdgeInsets.all(100.0),
                child: Column(
                  crossAxisAlignment: CrossAxisAlignment.start,
                  children: <Widget>[
                    Text(
                      "WAITING CAR ",
                      style: TextStyle(
                          fontSize: 30,
                          color: Colors.red,
                          fontWeight: FontWeight.bold),
                    ),
                    SizedBox(height: 50),
                    Container(
                      width: 150,
                      height: 70,
                      margin: EdgeInsets.symmetric(horizontal: 45),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ));
  }
}
