package com.example.flightbookingapp;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import java.io.Serializable;
import java.util.List;

import okhttp3.ResponseBody;
import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        EditText etFrom = findViewById(R.id.etFrom);
        EditText etTo   = findViewById(R.id.etTo);
        EditText etDate = findViewById(R.id.etDate);
        Button btnSearch = findViewById(R.id.btnSearch);

        ApiService api = ApiClient.getClient().create(ApiService.class);

        btnSearch.setOnClickListener(v -> {
            String from = etFrom.getText().toString().trim();
            String to   = etTo.getText().toString().trim();
            String date = etDate.getText().toString().trim();

            if(from.isEmpty() || to.isEmpty() || date.isEmpty()){
                Toast.makeText(this, "Please fill in all fields", Toast.LENGTH_SHORT).show();
                return;
            }

            api.searchFlights(from, to, date).enqueue(new Callback<FlightResponse>() {
                @Override
                public void onResponse(Call<FlightResponse> call, Response<FlightResponse> response) {
                    if(response.isSuccessful() && response.body() != null){
                        List<Flight> flights = response.body().flights;
                        Intent intent = new Intent(MainActivity.this, FlightResultsActivity.class);
                        intent.putExtra("flights", (Serializable) flights);
                        startActivity(intent);
                    }
                }

                @Override
                public void onFailure(Call<FlightResponse> call, Throwable t) {
                    Log.e("FLIGHTS", "Failed: " + t.getMessage());
                }
            });
        });
    }
}