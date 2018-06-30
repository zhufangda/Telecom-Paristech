package currency;


import android.app.Activity;
import android.util.Log;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

/**
 * Created by zhufa on 04/11/2017.
 */

public class IOUtils {
    /****
     * Get the data from a site specified by the arguments
     * @param url the http url
     * @return the inputStream of data
     */
    public static JSONObject getDateByHttp(String url) throws IOException {
        HttpURLConnection connection = null;
        StringBuilder stringBuilder = new StringBuilder();
        try {
            connection = (HttpURLConnection) new URL(url).openConnection();
            connection.setRequestMethod("GET");
            connection.setConnectTimeout(8000);
            connection.setReadTimeout(8000);
            InputStream inputStream = connection.getInputStream();

            try (BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream, "UTF-8"))) {
                String line = null;
                while ((line = reader.readLine()) != null) {
                    stringBuilder.append(line + "\n");
                }
            }
            Log.i("Currency", "Get infromation from " + connection.getURL().toString()
                    + " with response code " + connection.getResponseCode());
            return new JSONObject(stringBuilder.toString());
        }catch (MalformedURLException e) {
                e.printStackTrace();
        } catch (IOException e) {
                e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        } finally {
            if (connection != null) {
                    connection.disconnect();
                }
        }

        return null;
    }



        /***
     * Read json file by resourceId
     * @param context the activity containing the json file
     * @param resourceId the id of the JSON file
     * @return get Json object
     * @throws JSONException
     */
    public static JSONObject readJsonRessource(Activity context, int resourceId) throws JSONException {

        StringBuilder stringBuilder = new StringBuilder();

        try (InputStream inputStream = context.getResources().openRawResource(resourceId);
                BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream, "UTF-8"))) {

            String line = null;
            while ((line = reader.readLine()) != null) {
                stringBuilder.append(line + "\n");
            }

        } catch (IOException e) {
            System.err.print("Error reading rate:" + e.getLocalizedMessage());
        }

        return new JSONObject(stringBuilder.toString());
    }
}
