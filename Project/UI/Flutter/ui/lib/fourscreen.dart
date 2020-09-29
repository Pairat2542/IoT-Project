import 'dart:io';
import 'dart:convert';
import 'dart:async';
import 'package:flutter/material.dart';
import 'package:http/http.dart' as http;
import 'mainscreen.dart';

class Fourscreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
        appBar: AppBar(
          title: Text("ROVER NETWORK PROJECT"),
        ),
        body: SafeArea(
          child: Stack(
            children: <Widget>[
              // Background Image
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
                      "CAR IS ARRIVED",
                      style: TextStyle(
                          fontSize: 28,
                          color: Colors.red,
                          fontWeight: FontWeight.bold),
                    ),
                    SizedBox(height: 50),
                    Container(
                      width: 150,
                      height: 70,
                      margin: EdgeInsets.symmetric(horizontal: 45),
                      child: RaisedButton(
                        child: Text(
                          'EXIT',
                          style: TextStyle(color: Colors.white, fontSize: 20),
                        ),
                        color: Colors.deepPurple[900],
                        shape: RoundedRectangleBorder(
                            borderRadius: BorderRadius.circular(20)),
                        onPressed: () {
                          http
                              .get('http://192.168.43.145:8080/refresh')
                              .then((response) {
                            if (response.statusCode == 200) {
                              Navigator.pushReplacement(
                                  context,
                                  MaterialPageRoute(
                                      builder: (context) => MainScreen()));
                            } else {
                              throw Exception('Failed to load album');
                            }
                          });
                        },
                      ),
                    ),
                  ],
                ),
              ),
            ],
          ),
        ));
  }
}
