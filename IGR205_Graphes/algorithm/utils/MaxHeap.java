package utils;

public class MaxHeap {
	private HeapNode[] data;
	private int size;
	private int currentPos;

	public MaxHeap(int size) {
		this.size = size;
		this.data = new HeapNode[this.size + 1];
		this.currentPos = 1;
	}

	public void add(HeapNode value) {
		if (this.currentPos > this.size) {
			throw new IndexOutOfBoundsException(Integer.toString(this.size));
		}

		if (this.currentPos == 1) {
			this.data[this.currentPos++] = value;
		} else {
			int parent = this.currentPos / 2;
			int current = this.currentPos;

			while (parent >= 1) {
				if (this.data[parent].Dist < value.Dist) {
					this.data[current] = this.data[parent];
					current = parent;
					parent = parent / 2;
				} else {
					break;
				}
			}

			this.data[current] = value;
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
		int maxValue = 0;

		while (current < this.currentPos) {
			maxValue = this.data[current].Dist;
			if ((current + 1) < this.currentPos && this.data[current + 1].Dist > maxValue) {
				maxValue = this.data[++current].Dist;
			}

			if (temp.Dist >= maxValue) {
				break;
			} else {
				this.data[parent] = this.data[current];
				parent = current;
				current *= 2;
			}
		}

		this.data[parent] = temp;
		return valueReturn;
	}
}