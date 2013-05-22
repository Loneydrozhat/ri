package utils;

import java.util.HashMap;
import java.util.Iterator;


public class AccumulatorPool {

	private HashMap<Integer, Accumulator> pool;
	
	private final int maxEntries;
	
	public AccumulatorPool(int maxEntries) {
		this.maxEntries = maxEntries;
		this.pool = new HashMap<Integer, AccumulatorPool.Accumulator>();
	}

	public boolean hasAccumulator(Integer id) {
		return pool.containsKey(id);
	}

	public int size() {
		return pool.size();
	}
	
	public Iterator<Integer> keyIterator() {
		return pool.keySet().iterator();
	}
	
	public void addToAccumulator(Integer id, double v) {
		Accumulator a = pool.get(id);
		if (a == null && pool.size() >= maxEntries) {
			return;
		}
		if (a == null) {
			a = new Accumulator();
			pool.put(id, a);
		}
		a.add(v);
	}
	
	public void multiplyAccumulatorBy(Integer id, double v) {
		Accumulator a = pool.get(id);
		if (a != null) {
			a.multiplyBy(v);
		}
	}

	public double getValue(Integer id) {
		Accumulator a = pool.get(id);
		if (a == null) {
			return 0.0;
		}
		return a.value;
	}
	
	private static class Accumulator {
		double value = 0;
		void add(double v) {
			this.value += v;
		}
		void multiplyBy(double v) {
			this.value = v * this.value;
		}
	}
	
}
