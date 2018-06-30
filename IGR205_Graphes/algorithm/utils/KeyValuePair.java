package utils;

public class KeyValuePair implements Comparable<KeyValuePair> {
	public int key;
	public int value;

	public KeyValuePair() {
		super();
	}

	@Override
	public int compareTo(KeyValuePair o) {
		if (this.value != o.value)
			return this.value - o.value;

		return this.key - o.key;
	}

	@Override
	public String toString() {
		return "KeyValuePair [key=" + key + ", value=" + value + "]";
	}

}
