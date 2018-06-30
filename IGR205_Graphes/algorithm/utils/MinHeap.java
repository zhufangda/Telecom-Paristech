package utils;

public class MinHeap {
	public HeapNode[] data;
	public int[] pos;
	public int size;
	public int currentPos;

	public MinHeap(int size) {
		this.size = size;
		this.data = new HeapNode[this.size + 1];
		this.pos = new int[this.size + 1];
		this.currentPos = 1;
	}

	public void add(HeapNode value) {
		if (this.currentPos > this.size) {
			throw new IndexOutOfBoundsException(Integer.toString(this.size));
		}

		if (this.currentPos == 1) {
			this.data[this.currentPos++] = value;
			this.pos[value.VertexID] = this.currentPos - 1;
		} else {
			int parent = this.currentPos / 2;
			int current = this.currentPos;

			while (parent >= 1) {
				if (this.data[parent].Dist > value.Dist) {
					this.data[current] = this.data[parent];
					this.pos[this.data[parent].VertexID] = current;
					current = parent;
					parent = parent / 2;
				} else {
					break;
				}
			}

			this.data[current] = value;
			this.pos[value.VertexID] = current;
			this.currentPos++;
		}
	}

	public HeapNode delete() {

		if (this.currentPos <= 0) {
			throw new IndexOutOfBoundsException(Integer.toString(this.currentPos));
		}

		HeapNode valueReturn = this.data[1];

		HeapNode temp = data[--this.currentPos];
		int parent = 1;
		int current = 2;
		int minValue = 0;

		while (current < this.currentPos) {
			minValue = this.data[current].Dist;
			if ((current + 1) < this.currentPos && this.data[current + 1].Dist < minValue) {
				minValue = this.data[++current].Dist;
			}

			if (temp.Dist <= minValue) {
				break;
			} else {
				this.data[parent] = this.data[current];
				this.pos[this.data[current].VertexID] = parent;
				parent = current;
				current *= 2;
			}
		}

		this.data[parent] = temp;
		if (this.currentPos > 0)
			this.pos[temp.VertexID] = parent;

		return valueReturn;
	}

	public void update(HeapNode tempHeapNode) {
		int current = this.pos[tempHeapNode.VertexID];
		int parent = current / 2;

		while (parent >= 1) {
			if (this.data[parent].Dist > tempHeapNode.Dist) {
				this.data[current] = this.data[parent];
				this.pos[this.data[parent].VertexID] = current;
				current = parent;
				parent = parent / 2;
			} else {
				break;
			}
		}

		this.data[current] = tempHeapNode;
		this.pos[tempHeapNode.VertexID] = current;
	}
}