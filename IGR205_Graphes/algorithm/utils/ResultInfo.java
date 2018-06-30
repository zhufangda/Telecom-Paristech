package utils;

import java.util.Arrays;

public class ResultInfo implements Comparable<ResultInfo> {

	public int[] matchArr;
	public int num;
	public int len;
	public int[] scoreArr;
	public int upbound;
	public int pos;
	public int[] path;

	public ResultInfo(int len, int pos, int num) {
		super();
		this.len = len;
		this.scoreArr = new int[num];
		this.matchArr = new int[len];
		this.pos = pos;
		this.num = num;
		this.upbound = 0;
		this.path = new int[num];

		Arrays.fill(this.matchArr, -1);
		Arrays.fill(this.scoreArr, Integer.MAX_VALUE);
		Arrays.fill(this.path, Integer.MAX_VALUE);
	}

	public ResultInfo(ResultInfo tempResultInfo, int pos2) {
		this.len = tempResultInfo.len;
		this.scoreArr = new int[tempResultInfo.num];
		this.matchArr = new int[len];
		this.pos = pos2;
		this.num = tempResultInfo.num;
		this.upbound = tempResultInfo.upbound;
		this.path = new int[num];

		for (int i = 0; i < len; i++)
			this.matchArr[i] = tempResultInfo.matchArr[i];
		for (int i = 0; i < num; i++) {
			this.scoreArr[i] = tempResultInfo.scoreArr[i];
			this.path[i] = tempResultInfo.path[i];
		}
	}

	public int getScore() {
		int temp_score = 0;
		for (int i = 0; i < num; i++) {
			if (this.scoreArr[i] == Integer.MAX_VALUE)
				return this.scoreArr[i];
			else
				temp_score += this.scoreArr[i];
		}
		return temp_score;
	}

	@Override
	public int compareTo(ResultInfo o) {
		int temp_score = this.getScore() - o.getScore();
		if (temp_score != 0)
			return temp_score;
		return Arrays.toString(this.matchArr).compareTo(Arrays.toString(o.matchArr));
	}

	@Override
	public String toString() {
		return "ResultInfo [matchArr=" + Arrays.toString(matchArr) + ", socre=" + this.getScore() + ", path="
				+ Arrays.toString(this.path) + "]";
	}

	public boolean contains(int cur_id) {
		for (int i = 0; i < matchArr.length; i++) {
			if (matchArr[i] == cur_id)
				return true;
		}
		return false;
	}
}
