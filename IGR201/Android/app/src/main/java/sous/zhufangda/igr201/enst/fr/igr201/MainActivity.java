package sous.zhufangda.igr201.enst.fr.igr201;

import android.content.Intent;
import android.support.design.widget.TextInputEditText;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.ImageView;
import android.widget.Toast;


import java.math.BigDecimal;

import currency.Currency;


public class MainActivity extends AppCompatActivity implements View.OnClickListener{
    CurrencyItem currencyLeft = new CurrencyItem("USD",1.0, R.drawable.us);
    CurrencyItem currencyRight = new CurrencyItem("CNY", 6.628, R.drawable.cn);

    final int LEFT_CURRENCY = 1;
    final int RIGHT_CURRENCY = 2;

    ImageView flagLeft;
    ImageView exchange;
    ImageView flagRight;

    TextInputLayout layoutLeft;
    TextInputLayout layoutRight;

    TextInputEditText amountLeft;
    TextInputEditText amountRight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        flagLeft = (ImageView) findViewById(R.id.country_left);
        flagLeft.setOnClickListener(this);

        flagRight = (ImageView) findViewById(R.id.country_right);
        flagRight.setOnClickListener(this);

        exchange = (ImageView) findViewById(R.id.export_import);

        amountLeft = (TextInputEditText) findViewById(R.id.currency1);
        amountLeft.addTextChangedListener(new MyTextWatcher(amountLeft));

        amountRight = (TextInputEditText) findViewById(R.id.currency2);
        amountRight.setEnabled(false);

        layoutLeft = (TextInputLayout) findViewById(R.id.layoutLeft);
        layoutRight = (TextInputLayout) findViewById(R.id.layoutRight) ;

        exchange.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                CurrencyItem tmp = currencyLeft;
                currencyLeft = currencyRight;
                currencyRight = tmp;
                amountLeft.setText(amountRight.getText());
                updateInterface();
            }
        });

        updateInterface();

    }


    private void updateInterface(){
        flagLeft.setImageResource(currencyLeft.getImageRessourceId());
        flagRight.setImageResource(currencyRight.getImageRessourceId());
        layoutLeft.setHint(currencyLeft.getCurrency().getCurrencyCode());
        layoutRight.setHint(currencyRight.getCurrency().getCurrencyCode());
        amountLeft.setText(amountLeft.getText());
    }

    private String currencyChange(double amount_or, CurrencyItem or_currency, CurrencyItem des_currency){
        double amount = amount_or/or_currency.getRate() * des_currency.getRate();
        BigDecimal bd = new BigDecimal(amount);
        bd = bd.setScale(2, BigDecimal.ROUND_HALF_UP);
        return String.valueOf(bd);
    }

    @Override
    public void onClick(View v) {

        Intent intent = new Intent(MainActivity.this, CurrencyChooserActivity.class);
        if(v.equals(flagLeft)) startActivityForResult(intent, 1);
        if(v.equals(flagRight)) startActivityForResult(intent, 2);

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if(resultCode != RESULT_OK)
            Toast.makeText(this, "Don't change any currency type", Toast.LENGTH_SHORT).show();
        else if(requestCode == 1){
            currencyLeft = (CurrencyItem)data.getSerializableExtra("currency");
            updateInterface();
        }else{
            currencyRight = (CurrencyItem)data.getSerializableExtra("currency");
            updateInterface();
        }
    }

    private class MyTextWatcher implements TextWatcher {

        TextInputEditText view;

        public MyTextWatcher(TextInputEditText amountLeft) {
            this.view = amountLeft;
        }

        @Override
        public void beforeTextChanged(CharSequence s, int start, int count, int after) {

        }

        @Override
        public void onTextChanged(CharSequence s, int start, int before, int count) {

        }

        @Override
        public void afterTextChanged(Editable s) {
            try
            {
                double amount = Double.parseDouble(s.toString());
                amountRight.setText(currencyChange(amount
                            , currencyLeft
                            , currencyRight));
            }catch(NumberFormatException e){
                view.setError("Please type a valid amount");
            }

        }
    }

}



