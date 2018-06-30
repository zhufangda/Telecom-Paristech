package utils;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.TreeSet;

public class FPInfo implements Comparable<FPInfo> {
	public TreeSet<Integer> ItemSet;
	public int Frequency;
	public ArrayList<Integer> MatchingList;

	public FPInfo(TreeSet<Integer> fPStr, int frequency) {
		super();
		ItemSet = fPStr;
		MatchingList = new ArrayList<Integer>();
		Frequency = frequency;
	}

	@Override
	public String toString() {
		return "FPInfo [FPStr=" + ItemSet + ", Frequency=" + Frequency + ", MatchingList=" + MatchingList + "]";
	}

	@Override
	public int compareTo(FPInfo o) {
		return ItemSet.toString().compareTo(o.ItemSet.toString());
	}

	public boolean isSubFP(FPInfo o) {
		if (this.ItemSet.containsAll(o.ItemSet) && ((o.Frequency * 1.0 / this.Frequency) < 1.1)) {
			return true;
		}
		return false;
	}

	public String getFPStr() {
		String str = "";
		Iterator<Integer> it = this.ItemSet.iterator();
		while (it.hasNext()) {
			str += it.next() + " ";
		}
		return str.trim();
	}

	public int[] getItemArr() {
		int[] arr = new int[this.ItemSet.size()];
		Iterator<Integer> it = this.ItemSet.iterator();
		int i = 0;
		while (it.hasNext()) {
			arr[i] = it.next();
			i++;
		}
		return arr;
	}
}
