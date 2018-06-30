package utils;

public class NeighborInfo implements Comparable<NeighborInfo> {
	public int NeighborID;
	public int Label;
	public int Direction;
	public int Distance;

	public NeighborInfo(int neighborID, int label) {
		super();
		NeighborID = neighborID;
		Label = label;
	}

	@Override
	public String toString() {
		return "NeighborInfo [NeighborID=" + NeighborID + ", Label=" + Label + ", Direction=" + Direction
				+ ", Distance=" + Distance + "]";
	}

	@Override
	public int compareTo(NeighborInfo o) {
		if (Label != o.Label)
			return Label - o.Label;
		if (Direction != o.Direction)
			return Direction - o.Direction;

		return NeighborID - o.NeighborID;
	}
}
