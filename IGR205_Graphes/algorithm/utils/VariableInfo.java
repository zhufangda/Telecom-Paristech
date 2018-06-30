package utils;

import java.util.TreeSet;

public class VariableInfo implements Comparable<VariableInfo> {
	public String VariableStr;
	public TreeSet<Integer> matchesSet;

	public VariableInfo(String variableStr) {
		super();
		VariableStr = variableStr;
		matchesSet = new TreeSet<Integer>();
	}

	public void retain(VariableInfo o) {
		matchesSet.retainAll(o.matchesSet);
	}

	public void retainMatchSet(TreeSet<Integer> s) {
		matchesSet.retainAll(s);
	}

	public void addMatches(int curMatch) {
		matchesSet.add(curMatch);
	}

	public String getVariableStr() {
		return VariableStr;
	}

	public void setVariableStr(String variableStr) {
		VariableStr = variableStr;
	}

	public TreeSet<Integer> getMatchesSet() {
		return matchesSet;
	}

	public void setMatchesSet(TreeSet<Integer> matchesSet) {
		this.matchesSet = matchesSet;
	}

	public int size() {
		return this.matchesSet.size();
	}

	@Override
	public int compareTo(VariableInfo o) {
		return VariableStr.compareTo(o.VariableStr);
	}

	@Override
	public String toString() {
		return "VariableInfo [VariableStr=" + VariableStr + ", matchesSet=" + matchesSet + "]";
	}
}
