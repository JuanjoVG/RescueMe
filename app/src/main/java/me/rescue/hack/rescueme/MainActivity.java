package me.rescue.hack.rescueme;

import android.graphics.Color;
import android.os.Bundle;
import android.support.v4.app.FragmentActivity;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import com.google.android.gms.maps.GoogleMap;
import com.google.android.gms.maps.OnMapReadyCallback;
import com.google.android.gms.maps.SupportMapFragment;
import com.google.android.gms.maps.model.BitmapDescriptorFactory;
import com.google.android.gms.maps.model.LatLng;
import com.google.android.gms.maps.model.Marker;
import com.google.android.gms.maps.model.MarkerOptions;
import com.google.android.gms.maps.model.Polyline;
import com.google.android.gms.maps.model.PolylineOptions;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.Map;


public class MainActivity extends FragmentActivity implements OnMapReadyCallback, AdapterView.OnItemSelectedListener {

    private String user;
    private GoogleMap mMap;
    private TextView lastLocation;
    private List<String> users;
    private DatabaseReference mFirebaseDatabaseReference;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        // Obtain the SupportMapFragment and get notified when the map is ready to be used.
        SupportMapFragment mapFragment = (SupportMapFragment) getSupportFragmentManager()
                .findFragmentById(R.id.map);
        mapFragment.getMapAsync(this);

        lastLocation = (TextView) findViewById(R.id.last_location);

        users = new ArrayList<>();
        users.add("Select user");
        user = users.get(0);

        mFirebaseDatabaseReference = FirebaseDatabase.getInstance().getReference();

        mFirebaseDatabaseReference.child("users").addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                Map<String, Object> value = (Map<String, Object>) dataSnapshot.getValue();
                users.clear();
                users.addAll(value.keySet());
            }

            @Override
            public void onCancelled(DatabaseError databaseError) { }
        });

        Spinner spinner = (Spinner) findViewById(R.id.users);
        ArrayAdapter<String> spinnerAdapter = new ArrayAdapter<>(this,
                android.R.layout.simple_spinner_item, users);
        spinnerAdapter
                .setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(spinnerAdapter);
        spinner.setOnItemSelectedListener(this);
    }


    /**
     * Manipulates the map once available.
     * This callback is triggered when the map is ready to be used.
     * This is where we can add markers or lines, add listeners or move the camera. In this case,
     * we just add a marker near Sydney, Australia.
     * If Google Play services is not installed on the device, the user will be prompted to install
     * it inside the SupportMapFragment. This method will only be triggered once the user has
     * installed Google Play services and returned to the app.
     */
    @Override
    public void onMapReady(GoogleMap googleMap) {
        mMap = googleMap;
        if (!user.equals("Select user")) showPositions();
    }

    private void showPositions() {
        final List<Position> positions = new ArrayList<>();
        final Polyline polyline = mMap.addPolyline(new PolylineOptions().addAll(getLatLng(positions)).width(5).color(Color.BLUE));
        MarkerOptions markerOptions = new MarkerOptions().position(new LatLng(90.0, 180.0)).title("Última posición");
        final Marker marker = mMap.addMarker(markerOptions);

        mFirebaseDatabaseReference.child("users").child(user).child("positions").addChildEventListener(new ChildEventListener() {
            @Override
            public void onChildAdded(DataSnapshot dataSnapshot, String s) {
                Map<Integer, Object> value = (Map<Integer, Object>) dataSnapshot.getValue();
                Double lat = (Double) value.get("lat");
                Double lon = (Double) value.get("lon");
                Boolean alert = (Boolean) value.get("alert");
                Long time = (Long) value.get("time");
                LatLng latLng = new LatLng(lat, lon);
                Position position = new Position(latLng, alert);
                positions.add(position);
                polyline.setPoints(getLatLng(positions));
                marker.setPosition(latLng);
                if (alert != null && alert) {
                    mMap.addMarker(new MarkerOptions()
                            .position(latLng)
                            .title("Posición alerta")
                            .icon(BitmapDescriptorFactory.fromResource(R.drawable.alert_icon)));
                }
                if (time != null) {
                    String text = "Última ubicación: ";
                    Calendar calendar = GregorianCalendar.getInstance();
                    calendar.setTime(new Date(time));
                    text += calendar.get(Calendar.DAY_OF_MONTH) + "/";
                    text += calendar.get(Calendar.MONTH) + 1 + " ";
                    text += calendar.get(Calendar.HOUR_OF_DAY) + ":";
                    text += calendar.get(Calendar.MINUTE);
                    lastLocation.setText(text);
                }
            }

            @Override
            public void onChildChanged(DataSnapshot dataSnapshot, String s) { }

            @Override
            public void onChildRemoved(DataSnapshot dataSnapshot) { }

            @Override
            public void onChildMoved(DataSnapshot dataSnapshot, String s) { }

            @Override
            public void onCancelled(DatabaseError databaseError) { }
        });
    }

    private List<LatLng> getLatLng(List<Position> Positions) {
        List<LatLng> latLngs = new ArrayList<>();
        for (Position position : Positions) {
            latLngs.add(position.latLng);
        }
        return latLngs;
    }

    @Override
    public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
        user = parent.getItemAtPosition(position).toString();
        mMap.clear();
        showPositions();
    }

    @Override
    public void onNothingSelected(AdapterView<?> parent) { }

    private class Position {
        public LatLng latLng;
        public Boolean alert;

        public Position(LatLng latLng, Boolean alert) {
            this.latLng = latLng;
            this.alert = alert;
        }
    }
}
