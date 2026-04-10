package com.example.flightbookingapp;

import java.io.Serializable;

public class Flight implements Serializable {
    public int id;
    public String from;
    public String to;
    public String departure;
    public String arrival;
    public double price;
    public String airline;
    public int seats;
    public int duration;
}
