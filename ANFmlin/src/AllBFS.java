/*
 * Compute array where n[i] is number of ordered vertex pairs (v1, v2) at distance i
 * Takes undirected graph input from stdin with one edge per line: "nodeID1 nodeID2"
 * @author Rasmus Pagh
 * @version 2016.10.22
 * Usage: java AllBFS < graph.txt
 */

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.util.*;

public class AllBFS {

	private static Map<String,Set<String>> graph = new HashMap<String,Set<String>>();
	private static int[] n;

	private static void readGraph() {
		Scanner input = new Scanner(System.in);
		while (input.hasNextLine()) {
			String line = input.nextLine();
			String a = line.split(" ")[0];
			String b = line.split(" ")[1];
			graph.putIfAbsent(a, new HashSet<>());
			graph.get(a).add(b);
			graph.putIfAbsent(b, new HashSet<>());
			graph.get(b).add(a);
		}
	}

	private static void BFS(String start) {
		Queue<String> queue = new ArrayDeque<String>();
		Set<String> visited = new HashSet<String>();
		
		visited.add(start);
		queue.add(start);
		int d = 0;
		while (queue.peek()!=null) {
			int newNodes = queue.size();
			n[d++] += newNodes;
			for (int i=0; i<newNodes; i++) {
				String v = queue.poll();
				for (String w : graph.get(v)) {
					if (!visited.contains(w)) {
						visited.add(w);
						queue.add(w);
					}
				}
			}
		}
	}
	
	public static void main(String[] args) throws FileNotFoundException {
        if (args.length > 0) {
            System.setIn(new FileInputStream(args[0]));
        }

		readGraph();
		n = new int[graph.keySet().size()];

		for (String v : graph.keySet()) { BFS(v); }

		for (int i=0; i<n.length; i++) {
			if (n[i]>0) System.out.println("n["+i+"] = "+n[i]);
		}
		
	}
}
