package utils;

public class HeapNode implements Comparable<HeapNode> {
	public int VertexID;
	public int Dist;

	public HeapNode(int vertexID) {
		super();
		VertexID = vertexID;
		Dist = Integer.MAX_VALUE;
	}

	public HeapNode(HeapNode key) {
		VertexID = key.VertexID;
		Dist = key.Dist;
	}

	@Override
	public int compareTo(HeapNode o) {
		if (this.Dist != o.Dist)
			return this.Dist - o.Dist;
		return this.VertexID - o.VertexID;
	}

	@Override
	public String toString() {
		return "HeapNode [VertexID=" + VertexID + ", Dist=" + Dist + "]";
	}
}