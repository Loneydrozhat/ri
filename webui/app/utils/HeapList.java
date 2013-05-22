package utils;

import java.util.ArrayList;

public class HeapList<E extends Comparable<E>> extends ArrayList<E> {

	private final int maxSize;
	boolean heapBuilt = false;
	
	public HeapList(int maxSize) {
		this.maxSize = maxSize;
	}

	private void minHeapify(int pos, int n) {
		int max = 2 * pos + 1;

		if (max < n) {
			if ((max + 1) < n && compare(this.get(max), this.get(max + 1)) > 0) {
				++max;
			}
			if (compare(this.get(max), this.get(pos)) < 0) {
				E temp = this.get(max);
				this.set(max, this.get(pos));
				this.set(pos, temp);
				minHeapify(max, n);
			}
		}
	}

	private void buildMinHeap() {
		if (heapBuilt) {
			return;
		}
		heapBuilt = true;
		int size = this.size();
		int i;

		for (i = size / 2 - 1; i >= 0; --i) {
			minHeapify(i, size);
		}
	}

	public E popMin() {
		buildMinHeap();
		E min = this.get(0);
		this.set(0, this.get(this.size() - 1));
		this.remove(this.size() - 1);
		minHeapify(0, this.size());
		return min;
	}

	public void pushAndReplaceMin(E newElem) {
		if (this.size() < maxSize) {
			this.add(newElem);
		} else {
			if (this.size() == maxSize) {
				buildMinHeap();
			}
			E min = this.get(0);
			this.set(0, newElem);
			minHeapify(0, this.size());
		}
	}

	private int compare(E e1, E e2) {
		return e1.compareTo(e2);
	}
}
