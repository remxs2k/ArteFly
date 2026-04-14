package com.example.flightbookingapp;

import android.app.DatePickerDialog;
import android.content.Intent;
import android.os.Bundle;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;
import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;

public class MainActivity extends AppCompatActivity {

    private EditText etFrom, etTo, etDate;
    private Button btnSearch;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        etFrom   = findViewById(R.id.etFrom);
        etTo     = findViewById(R.id.etTo);
        etDate   = findViewById(R.id.etDate);
        btnSearch = findViewById(R.id.btnSearch);

        etDate.setOnClickListener(v -> {
            final Calendar c = Calendar.getInstance();
            int year  = c.get(Calendar.YEAR);
            int month = c.get(Calendar.MONTH);
            int day   = c.get(Calendar.DAY_OF_MONTH);

            DatePickerDialog datePickerDialog = new DatePickerDialog(
                    MainActivity.this,
                    (view, selectedYear, selectedMonth, selectedDay) -> {
                        String formattedDate = String.format(
                                "%04d-%02d-%02d", selectedYear, selectedMonth + 1, selectedDay);
                        etDate.setText(formattedDate);
                    },
                    year, month, day);

            datePickerDialog.getDatePicker().setMinDate(System.currentTimeMillis() - 1000);
            datePickerDialog.show();
        });

        btnSearch.setOnClickListener(v -> {
            String from = etFrom.getText().toString().trim();
            String to   = etTo.getText().toString().trim();
            String date = etDate.getText().toString().trim();

            if (from.isEmpty() || to.isEmpty() || date.isEmpty()) {
                Toast.makeText(this, "Te rog completează toate câmpurile", Toast.LENGTH_SHORT).show();
                return;
            }

            // Disable button to prevent double-tap while loading
            btnSearch.setEnabled(false);

            ApiService apiService = ApiClient.getClient().create(ApiService.class);
            apiService.searchFlights(from, to, date).enqueue(new Callback<FlightResponse>() {

                @Override
                public void onResponse(Call<FlightResponse> call, Response<FlightResponse> response) {
                    btnSearch.setEnabled(true);

                    if (!response.isSuccessful() || response.body() == null) {
                        Toast.makeText(MainActivity.this,
                                "Eroare server: " + response.code(), Toast.LENGTH_SHORT).show();
                        return;
                    }

                    List<Flight> flights = response.body().flights;

                    if (flights == null || flights.isEmpty()) {
                        Toast.makeText(MainActivity.this,
                                "Nu s-au găsit zboruri.", Toast.LENGTH_SHORT).show();
                        return;
                    }

                    Intent intent = new Intent(MainActivity.this, FlightResultsActivity.class);
                    intent.putExtra("flights", new ArrayList<>(flights));
                    startActivity(intent);
                }

                @Override
                public void onFailure(Call<FlightResponse> call, Throwable t) {
                    btnSearch.setEnabled(true);
                    Toast.makeText(MainActivity.this,
                            "Conexiune eșuată: " + t.getMessage(), Toast.LENGTH_LONG).show();
                }
            });
        });
    }
}