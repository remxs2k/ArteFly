package com.example.flightbookingapp;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.Query;

public interface ApiService {
    @GET("ping")
    Call<ResponseBody> ping();

    @GET("search")
    Call<FlightResponse> searchFlights(
            @Query("from") String from,
            @Query("to") String to,
            @Query("date") String date
    );
}
