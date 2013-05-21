package utils;

import java.util.HashMap;


public class AccumulatorPool {

	private HashMap<Integer, Accumulator> pool;
	
	public boolean hasAccumulator(Integer id) {
		return pool.containsKey(id);
	}
	
	public void addToAccumulator(Integer id, double v) {
		Accumulator a = pool.get(id);
		if (a == null) {
			a = new Accumulator();
			pool.put(id, a);
		}
		a.add(v);
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
		public void add(double v) {
			this.value += v;
		}
	}
}
