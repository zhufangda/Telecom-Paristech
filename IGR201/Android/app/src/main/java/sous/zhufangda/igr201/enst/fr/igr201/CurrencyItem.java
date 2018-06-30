package sous.zhufangda.igr201.enst.fr.igr201;


import java.io.Serializable;

import currency.Currency;

/**
 * Created by zhufa on 03/11/2017.
 * This class contain all the information about a currency.
 * For example, country code, currency code, change rate, the national flag
 */

public class CurrencyItem implements Serializable{
    private Currency currency;
    private int imageId;
    private double rate;

    public CurrencyItem(String currencyCode, double rate, int imageId){
        this.currency = Currency.getInstance(currencyCode);
        this.imageId = imageId;
        this.rate = rate;
    }

    public Currency getCurrency(){
        return this.currency;
    }
    public int getImageRessourceId(){
        return imageId;
    }
    public double getRate(){
        return this.rate;
    }
}
