package com.example.flightbookingapp;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import java.util.List;

public class FlightAdapter extends RecyclerView.Adapter<FlightAdapter.FlightViewHolder> {

    private List<Flight> flights;
    private Context context;

    public FlightAdapter(Context context, List<Flight> flights) {
        this.context = context;
        this.flights = flights;
    }

    @NonNull
    @Override
    public FlightViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(context).inflate(R.layout.item_flight, parent, false);
        return new FlightViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull FlightViewHolder holder, int position) {
        Flight flight = flights.get(position);
        holder.tvRoute.setText(flight.from + " → " + flight.to);
        holder.tvTimes.setText(flight.departure + " - " + flight.arrival);
        holder.tvAirline.setText(flight.airline);
        holder.tvPrice.setText("$" + flight.price);
    }

    @Override
    public int getItemCount() {
        return flights.size();
    }

    public static class FlightViewHolder extends RecyclerView.ViewHolder {
        TextView tvRoute, tvTimes, tvAirline, tvPrice;

        public FlightViewHolder(@NonNull View itemView) {
            super(itemView);
            tvRoute   = itemView.findViewById(R.id.tvRoute);
            tvTimes   = itemView.findViewById(R.id.tvTimes);
            tvAirline = itemView.findViewById(R.id.tvAirline);
            tvPrice   = itemView.findViewById(R.id.tvPrice);
        }
    }
}