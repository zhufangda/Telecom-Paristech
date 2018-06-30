package sous.zhufangda.igr201.enst.fr.igr201;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.Toast;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import currency.Currency;
import currency.IOUtils;

public class CurrencyChooserActivity extends AppCompatActivity {
    private static final int ON_LINE = 1;
    private static final int HORS_LINR = 2;
    private JSONObject rates;

    private List<CurrencyItem> currencyList = new ArrayList<>();
    private Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg){
            switch (msg.what) {
                case ON_LINE:
                    rates = (JSONObject) msg.obj;
                    Toast.makeText(CurrencyChooserActivity.this, "Network is available, get data from Internet", Toast.LENGTH_LONG).show();
                    break;
                case HORS_LINR:
                    rates = (JSONObject) msg.obj;
                    Toast.makeText(CurrencyChooserActivity.this, "Network is not available, get off-line data", Toast.LENGTH_LONG).show();
                    break;
            }

            try {
                init_list();
            } catch (JSONException e) {
                e.printStackTrace();
            }

            CurrencyAdapter currencyAdapter = new CurrencyAdapter(CurrencyChooserActivity.this, R.layout.currency_item, currencyList);
            ListView currency_List = (ListView) findViewById(R.id.currency_list);
            currency_List.setOnItemClickListener(new AdapterView.OnItemClickListener() {
                @Override
                public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                    CurrencyItem currencyItem = currencyList.get(position);
                    Intent intent = getIntent();
                    intent.putExtra("currency", currencyItem);
                    setResult(RESULT_OK,intent);
                   CurrencyChooserActivity.this.finish();
                }
            });
            currency_List.setAdapter(currencyAdapter);
      }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_currency_chooser);
        getCurrencyWithHttp();


    }


    private void init_list() throws JSONException {
        //rates = IOUtils.readJsonRessource(this,R.raw.taux_2017_11_02).getJSONObject("rates");
        rates = rates.getJSONObject("rates");
        JSONObject countryCodeJson = IOUtils.readJsonRessource(this, R.raw.country);

        if (!currencyList.isEmpty()) currencyList.clear();
        JSONArray names = rates.names();
        String currencyCode, countryCode;

        for (int i = 0; i < names.length(); i++) {
            currencyCode = names.getString(i).trim();
            countryCode = countryCodeJson.getString(currencyCode);
            currencyList.add(new CurrencyItem(currencyCode, rates.getDouble(currencyCode), getResource(countryCode.toLowerCase())));
        }
    }

    /**
     * Get identify of image in the drawable by its name
     * @param imageName
     * @return the id resource of the image
     */
    public int getResource(String imageName) {
        Context ctx = getBaseContext();
        int resId = getResources().getIdentifier(imageName, "drawable", ctx.getPackageName());

        return resId;
    }


    private void getCurrencyWithHttp(){

        new Thread(new Runnable() {
            String appId = "a48c91bde57144c3b8ac4d482dc63b4b";

            @Override
            public void run() {

                try {
                    Message message = new Message();
                    if(isNetworkAvailable(CurrencyChooserActivity.this)){
                        Log.i("Network", "Network is available");
                        message.what = ON_LINE;
                        message.obj = IOUtils.getDateByHttp("https://openexchangerates.org/api/latest.json?app_id=" + appId);
                    }else{
                        Log.i("Network", "Network is not available");
                        message.what = HORS_LINR;
                        message.obj = IOUtils.readJsonRessource(CurrencyChooserActivity.this,R.raw.taux_2017_11_02);
                    }
                    handler.sendMessage(message);
                }  catch (IOException e) {
                    e.printStackTrace();
                } catch (JSONException e) {
                    e.printStackTrace();
                }

            }
        }).start();
    }


    /**
     * Check state of network
     * @param context Context
     * @return true if the network is available
     */
    public boolean isNetworkAvailable(Context context) {
        ConnectivityManager connMgr = (ConnectivityManager)
                getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo networkInfo = connMgr.getActiveNetworkInfo();
        return (networkInfo != null && networkInfo.isConnected());
    }



}
