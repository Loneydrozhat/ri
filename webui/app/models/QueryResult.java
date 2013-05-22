package models;

public class QueryResult implements Comparable<QueryResult> {

	public final String url;
	public final double weight;
	public QueryResult(String url, double weight) {
		this.url = url;
		this.weight = weight;
	}
	
	@Override
	public int compareTo(QueryResult arg0) {
		return Double.compare(this.weight, arg0.weight);
	}
	
}
