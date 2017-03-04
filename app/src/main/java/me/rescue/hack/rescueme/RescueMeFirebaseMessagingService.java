package me.rescue.hack.rescueme;

import android.app.PendingIntent;
import android.content.Intent;
import android.util.Log;

import com.google.firebase.messaging.FirebaseMessagingService;
import com.google.firebase.messaging.RemoteMessage;

public class RescueMeFirebaseMessagingService extends FirebaseMessagingService {
    private static final String TAG = "OAFBMessagingService";
    public static final String LOCATION_CHILD = "locations";
    @Override
    public void onMessageReceived(RemoteMessage remoteMessage) {
        // Handle data payload of FCM messages.
        Log.d(TAG, "FCM Message Id: " + remoteMessage.getMessageId());
        Log.d(TAG, "FCM Notification Message: " +
                remoteMessage.getNotification());
        Log.d(TAG, "FCM Data Message: " + remoteMessage.getData());
        //Calling method to generate notification
//        sendNotification(remoteMessage.getData().get("message"));
//        DatabaseReference mFirebaseDatabaseReference = FirebaseDatabase.getInstance().getReference();
//        mFirebaseDatabaseReference.child(LOCATION_CHILD).push().setValue(remoteMessage.getData().get("lat") + "," + remoteMessage.getData().get("lon"));
    }
    //This method is only generating push notification
    //It is same as we did in earlier posts
//    private void sendNotification(String message) {
//        Intent intent = new Intent(Intent.ACTION_CALL, Uri.parse("tel:" + "659984265"));
//        PendingIntent contentIntent = PendingIntent.getActivity(this, 0, intent, PendingIntent.FLAG_ONE_SHOT);
//        Intent intent2 = new Intent(this, MapsActivity.class);
//        PendingIntent contentIntent2 = PendingIntent.getActivity(this, 0, intent2, PendingIntent.FLAG_ONE_SHOT);
//        Bitmap largeIcon = BitmapFactory.decodeResource(getResources(), R.drawable.hans);
//        Notification notification = new NotificationCompat.Builder(this)
//                .setCategory(Notification.CATEGORY_PROMO)
//                .setContentTitle("OAssis Alert")
//                .setContentText("Hans Moleman "  + message)
//                .setSmallIcon(R.drawable.ic_notification)
//                .setLargeIcon(largeIcon)
//                .setAutoCancel(true)
//                .addAction(android.R.drawable.ic_menu_call, "Call Now", contentIntent)
//                .addAction(android.R.drawable.ic_menu_mapmode, "Show Map", contentIntent2)
//                .setContentIntent(contentIntent2)
//                .setPriority(Notification.PRIORITY_HIGH)
//                .setVibrate(new long[]{1000, 1000, 1000, 1000, 1000})
//                .build();
//        NotificationManager notificationManager =
//                (NotificationManager) getSystemService(NOTIFICATION_SERVICE);
//        notificationManager.notify(0, notification);
//    }
}