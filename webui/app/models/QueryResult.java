package models;

public class QueryResult implements Comparable<QueryResult> {

	public final String url;
	public final double weight;
	public final double pr;
	public QueryResult(String url, double weight, double pr) {
		this.url = url;
		this.weight = weight;
		this.pr = pr;
	}
	
	@Override
	public int compareTo(QueryResult arg0) {
		return Double.compare(this.weight, arg0.weight);
	}
	
}
