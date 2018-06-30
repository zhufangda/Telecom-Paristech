package sous.zhufangda.igr201.enst.fr.igr201;

import android.content.Context;
import android.support.annotation.LayoutRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;

/**
 * Created by zhufa on 03/11/2017.
 * Un adapter for the <code>currencyItem</code>
 */

public class CurrencyAdapter extends ArrayAdapter<CurrencyItem> {
    private int ressourceId;

    public CurrencyAdapter(@NonNull Context context, @LayoutRes int resource, @NonNull List<CurrencyItem> objects) {
        super(context, resource, objects);
        this.ressourceId = resource;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        CurrencyItem currency = getItem(position);
        View view = LayoutInflater.from(getContext()).inflate(ressourceId, null);

        ImageView countryFlag = (ImageView)view.findViewById(R.id.country_flag);
        countryFlag.setImageResource(currency.getImageRessourceId());

        TextView currencyCode_text = (TextView) view.findViewById(R.id.currency_code);
        currencyCode_text.setText(currency.getCurrency().getCurrencyCode());

        TextView displayName = (TextView) view.findViewById(R.id.display_name);
        displayName.setText(currency.getCurrency().getDisplayName());

        TextView rate = (TextView) view.findViewById(R.id.rate);
        rate.setText(String.valueOf(currency.getRate()) + " " + currency.getCurrency().getSymbol());

        return view;
    }
}
