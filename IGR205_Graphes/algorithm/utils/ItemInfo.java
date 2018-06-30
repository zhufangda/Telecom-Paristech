package utils;

public class ItemInfo implements Comparable<ItemInfo> {
	public int Label;
	public int Direction;
	public int Count;

	public ItemInfo(int label, int direction, int count) {
		super();
		Label = label;
		Direction = direction;
		Count = count;
	}

	@Override
	public String toString() {
		return Label + "," + Direction + "," + Count;
	}

	@Override
	public int compareTo(ItemInfo o) {
		if (Label != o.Label)
			return Label - o.Label;
		if (Direction != o.Direction)
			return Direction - o.Direction;

		return Count - o.Count;
	}
}
