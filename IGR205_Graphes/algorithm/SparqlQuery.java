import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.PrintStream;
import java.io.Reader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Map.Entry;
import java.util.TreeMap;
import java.util.TreeSet;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.queryParser.ParseException;
import org.apache.lucene.queryParser.QueryParser;
import org.apache.lucene.search.Hits;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.openrdf.query.algebra.StatementPattern;
import org.openrdf.query.algebra.helpers.StatementPatternCollector;
import org.openrdf.query.parser.ParsedQuery;
import org.openrdf.query.parser.sparql.SPARQLParser;

import com.sleepycat.je.Database;
import com.sleepycat.je.DatabaseConfig;
import com.sleepycat.je.DatabaseEntry;
import com.sleepycat.je.Environment;
import com.sleepycat.je.EnvironmentConfig;
import com.sleepycat.je.LockMode;
import com.sleepycat.je.OperationStatus;

import utils.FPInfo;
import utils.ItemInfo;
import utils.NeighborInfo;
import utils.HeapNode;
import utils.VariableInfo;
import utils.ResultInfo;

@SuppressWarnings("deprecation")
public class SparqlQuery {
	public static void main(String[] args) {
		if (args.length != 2) {
			System.out.println("usage:");
			System.out.println("================");
			System.out.println("java -classpath algo.jar SparqlQuery <keyword+query file location> <top k>");
			System.out.println("");
			System.out.println("for example,");
			System.out.println("java -classpath algo.jar SparqlQuery data/sembib_Q1.txt 10");
			System.exit(0);
		}
		try {
			String str = "";
			int cur_id = 0;
			String[] TermArr, TermArr1;
			String dir_index = "index";
			int TopKNum = Integer.valueOf(args[1]);

			Date loadingStartTime = new Date();

			EnvironmentConfig envConfig1 = new EnvironmentConfig();
			envConfig1.setAllowCreate(true);
			Environment myDbEnvironment1 = new Environment(new File(dir_index + "/DistanceIndex"), envConfig1);

			DatabaseConfig dbConfig1 = new DatabaseConfig();
			dbConfig1.setAllowCreate(true);
			Database myDatabase1 = myDbEnvironment1.openDatabase(null, "DistanceIndexDB", dbConfig1);

			EnvironmentConfig envConfig2 = new EnvironmentConfig();
			envConfig2.setAllowCreate(true);
			Environment myDbEnvironment2 = new Environment(new File(dir_index + "/StructuralIndex"), envConfig2);

			DatabaseConfig dbConfig2 = new DatabaseConfig();
			dbConfig2.setAllowCreate(true);
			Database myDatabase2 = myDbEnvironment2.openDatabase(null, "StructuralIndexDB", dbConfig2);

			/**************** Beginning of loading data ******************/

			System.out.println("loading adjacent list...");
			InputStream in = new FileInputStream(new File(dir_index + "/graph_adjacent_list.txt"));
			Reader inr = new InputStreamReader(in);
			BufferedReader br = new BufferedReader(inr);

			int neighbor_id = 0, label_id = 0, NodeNum = 0;
			int updateNum = 2;

			str = br.readLine();
			NodeNum = Integer.valueOf(str);
			NeighborInfo[][] adjacentList = new NeighborInfo[NodeNum][];

			str = br.readLine();
			while (str != null) {

				str = str.trim();
				TermArr = str.split("\t");
				cur_id = Integer.valueOf(TermArr[0]);
				TermArr1 = TermArr[1].split(" ");
				// (neighbor_id, label_id, distance, direction)
				adjacentList[cur_id] = new NeighborInfo[TermArr1.length / 4];
				for (int i = 0; i < TermArr1.length; i = i + 4) {
					neighbor_id = Integer.valueOf(TermArr1[i]);
					label_id = Integer.valueOf(TermArr1[i + 1]);

					adjacentList[cur_id][i / 4] = new NeighborInfo(neighbor_id, label_id);
					adjacentList[cur_id][i / 4].Distance = Integer.valueOf(TermArr1[i + 2]);
					adjacentList[cur_id][i / 4].Direction = Integer.valueOf(TermArr1[i + 3]);
				}

				str = br.readLine();
			}

			br.close();

			System.out.println("loading entity_id_map...");
			HashMap<Integer, String> IDEntityMap = new HashMap<Integer, String>();
			HashMap<String, Integer> EntityIDMap = new HashMap<String, Integer>();
			byte[] entityTagArr = new byte[NodeNum];

			InputStream in_entity = new FileInputStream(new File(dir_index + "/entity_id_map.txt"));
			Reader inr_entity = new InputStreamReader(in_entity);
			BufferedReader br_entity = new BufferedReader(inr_entity);

			str = br_entity.readLine();
			// entity tag: 1 means entities, 0 means data values
			Arrays.fill(entityTagArr, (byte) 0);

			while (str != null) {
				TermArr = str.split("\t");
				cur_id = Integer.valueOf(TermArr[0]);
				EntityIDMap.put(TermArr[1], cur_id);
				IDEntityMap.put(cur_id, TermArr[1]);

				if (TermArr[1].startsWith("<")) {
					entityTagArr[cur_id] = 1;
				}

				str = br_entity.readLine();
			}

			br_entity.close();

			System.out.println("loading distance-based index ...");
			InputStream in175 = new FileInputStream(new File(dir_index + "/pivot.txt"));
			Reader inr175 = new InputStreamReader(in175);
			BufferedReader br175 = new BufferedReader(inr175);

			str = br175.readLine();
			cur_id = 0;
			HashSet<Integer> RNSet = new HashSet<Integer>();

			while (str != null) {

				TermArr = str.split("\t");
				cur_id = Integer.valueOf(TermArr[0]);

				RNSet.add(cur_id);

				str = br175.readLine();
			}
			br175.close();

			System.out.println("loading predicate weights...");
			InputStream in650 = new FileInputStream(new File(dir_index + "/p_weight.txt"));
			Reader inr650 = new InputStreamReader(in650);
			BufferedReader br650 = new BufferedReader(inr650);

			TreeMap<String, Integer> PredicateIDMap = new TreeMap<String, Integer>();
			TreeMap<Integer, String> IDPredicateMap = new TreeMap<Integer, String>();
			TreeMap<Integer, ItemInfo> IDItemMap = new TreeMap<Integer, ItemInfo>();
			TreeMap<Integer, Integer> LabelItemPosMap = new TreeMap<Integer, Integer>();
			int item_id = 0, p_id = 0;

			str = br650.readLine();
			while (str != null) {
				TermArr = str.split(" ");

				PredicateIDMap.put(TermArr[0], Integer.valueOf(TermArr[1]));
				IDPredicateMap.put(Integer.valueOf(TermArr[1]), TermArr[0]);

				p_id = Integer.valueOf(TermArr[1]);
				LabelItemPosMap.put(p_id, item_id);

				IDItemMap.put(item_id, new ItemInfo(p_id, 0, 1));
				IDItemMap.put((item_id + 1), new ItemInfo(p_id, 0, 2));
				IDItemMap.put((item_id + 2), new ItemInfo(p_id, 0, 3));

				IDItemMap.put((item_id + 3), new ItemInfo(p_id, 1, 1));
				IDItemMap.put((item_id + 4), new ItemInfo(p_id, 1, 2));
				IDItemMap.put((item_id + 5), new ItemInfo(p_id, 1, 3));

				item_id = item_id + 6;

				str = br650.readLine();
			}

			br650.close();

			System.out.println("loading structural index...");
			InputStream in70 = new FileInputStream(new File(dir_index + "/fp.out"));
			Reader inr70 = new InputStreamReader(in70);
			BufferedReader br70 = new BufferedReader(inr70);
			TreeSet<Integer> frequentPropertySet = new TreeSet<Integer>();
			TreeSet<Integer> frequentItemSet = new TreeSet<Integer>();
			TreeMap<Integer, ArrayList<Integer>> ItemFPListMap = new TreeMap<Integer, ArrayList<Integer>>();
			ArrayList<FPInfo> fpList = new ArrayList<FPInfo>();
			String fpStr = "";
			int cur_frequency = 0;

			str = br70.readLine();
			while (str != null) {
				TermArr = str.split(":");

				TermArr1 = TermArr[0].split(" ");
				TreeSet<Integer> curItemSet = new TreeSet<Integer>();
				for (int i = 0; i < TermArr1.length; i++) {
					TermArr1[i] = TermArr1[i].trim();
					item_id = Integer.valueOf(TermArr1[i]);
					ItemInfo curItem = IDItemMap.get(item_id);
					frequentPropertySet.add(curItem.Label);
					frequentItemSet.add(item_id);

					curItemSet.add(item_id);
				}

				TermArr[1] = TermArr[1].trim();
				cur_frequency = Integer.valueOf(TermArr[1]);

				FPInfo newFP = new FPInfo(curItemSet, cur_frequency);

				if (fpList.size() == 0) {
					fpList.add(newFP);
				} else {
					int fp_tag = 0;
					for (int i = fpList.size() - 1; i >= 0; i--) {
						if (newFP.isSubFP(fpList.get(i))) {
							fp_tag = 1;
							break;
						}
					}
					if (fp_tag == 0) {
						fpList.add(newFP);
					}
				}

				str = br70.readLine();
			}

			br70.close();

			for (int i = 0; i < fpList.size(); i++) {
				int[] curItemArr = fpList.get(i).getItemArr();

				for (int j = 0; j < curItemArr.length; j++) {
					if (!ItemFPListMap.containsKey(curItemArr[j])) {
						ItemFPListMap.put(curItemArr[j], new ArrayList<Integer>());
					}
					ItemFPListMap.get(curItemArr[j]).add(i);
				}
			}

			Date loadingEndTime = new Date();
			System.out.print("loading time:");
			System.out.println(loadingEndTime.getTime() - loadingStartTime.getTime() + "ms");

			/**************** End of loading data ******************/

			System.out.println("begin to process keyword...");
			String fileStr = args[0];
			System.out.println("file location: " + fileStr);
			InputStream in1 = new FileInputStream(new File(fileStr));
			Reader inr1 = new InputStreamReader(in1);
			BufferedReader br1 = new BufferedReader(inr1);

			Date currentTime1 = new Date();
			str = br1.readLine();
			System.out.println("keyword: " + str);
			TermArr = str.split(";");
			int[][] visited = new int[TermArr.length][NodeNum];
			int[][] candidateDist = new int[TermArr.length][NodeNum];
			byte[] b = null;
			NeighborInfo[] TermArr2 = null;
			int keyword_count = 0;

			String[] keywordArr = new String[TermArr.length];
			for (int n = 0; n < keywordArr.length; n++) {
				TermArr[n] = TermArr[n].trim();
				keywordArr[n] = TermArr[n];

				Arrays.fill(visited[n], Integer.MAX_VALUE);
				Arrays.fill(candidateDist[n], Integer.MAX_VALUE);

				HashSet<Integer> visitedRNSet = new HashSet<Integer>();
				TreeSet<HeapNode> candidate = new TreeSet<HeapNode>();

				Hits hits = null;
				Query query = null;
				IndexSearcher searcher = new IndexSearcher(dir_index + "/LuceneIndex");
				Analyzer analyzer = new StandardAnalyzer();
				try {
					QueryParser qp = new QueryParser("body", analyzer);
					query = qp.parse(keywordArr[n]);
				} catch (ParseException e) {
				}
				if (searcher != null) {
					hits = searcher.search(query);

					for (int m = 0; m < hits.length(); m++) {

						String titleStr = hits.doc(m).getField("title").stringValue();
						cur_id = Integer.valueOf(titleStr);

						HeapNode curHeapNode = new HeapNode(cur_id);
						curHeapNode.Dist = 0;
						candidateDist[n][cur_id] = 0;

						candidate.add(curHeapNode);
						keyword_count++;
					}

					while (candidate.size() != 0) {
						HeapNode curHeapNode = candidate.pollFirst();
						visited[n][curHeapNode.VertexID] = curHeapNode.Dist;
						keyword_count++;

						if (RNSet.contains(curHeapNode.VertexID) && !visitedRNSet.contains(curHeapNode.VertexID)
								&& visitedRNSet.size() < updateNum) {

							String aKey = "" + curHeapNode.VertexID;
							DatabaseEntry theKey = new DatabaseEntry(aKey.getBytes("UTF-8"));
							DatabaseEntry theData = new DatabaseEntry();

							if (myDatabase1.get(null, theKey, theData, LockMode.DEFAULT) == OperationStatus.SUCCESS) {
								// Recreate the data String.
								b = theData.getData();
							}

							visitedRNSet.add(curHeapNode.VertexID);
							for (int k = 0; k < NodeNum; k++) {
								if (b[k] != -1) {
									HeapNode newHeapNode = new HeapNode(k);
									newHeapNode.Dist = b[k] + curHeapNode.Dist;

									if (candidateDist[n][k] != Integer.MAX_VALUE) {
										int cur_dist = candidateDist[n][k];
										if (newHeapNode.Dist < cur_dist) {
											candidateDist[n][k] = newHeapNode.Dist;
										}
									} else {
										candidateDist[n][k] = newHeapNode.Dist;
									}

								}
							}
							continue;
						}

						TermArr2 = adjacentList[curHeapNode.VertexID];
						for (int i = 0; i < TermArr2.length; i++) {

							if (visited[n][TermArr2[i].NeighborID] != Integer.MAX_VALUE)
								continue;
							if (entityTagArr[TermArr2[i].NeighborID] == 0)
								continue;

							if (candidateDist[n][TermArr2[i].NeighborID] != Integer.MAX_VALUE) {

								int cur_dist = candidateDist[n][TermArr2[i].NeighborID];
								if (TermArr2[i].Distance + curHeapNode.Dist < cur_dist) {
									HeapNode tempHeapNode = new HeapNode(TermArr2[i].NeighborID);
									tempHeapNode.Dist = cur_dist;
									candidate.remove(tempHeapNode);

									HeapNode newHeapNode = new HeapNode(TermArr2[i].NeighborID);
									newHeapNode.Dist = TermArr2[i].Distance + curHeapNode.Dist;
									candidate.add(newHeapNode);

									candidateDist[n][TermArr2[i].NeighborID] = TermArr2[i].Distance + curHeapNode.Dist;
								}
							} else {
								HeapNode newHeapNode = new HeapNode(TermArr2[i].NeighborID);
								newHeapNode.Dist = TermArr2[i].Distance + curHeapNode.Dist;
								candidate.add(newHeapNode);
								candidateDist[n][TermArr2[i].NeighborID] = TermArr2[i].Distance + curHeapNode.Dist;
							}
						}
					}

				}
			}

			System.out.print("candidateDist time:");
			Date currentTime2 = new Date();
			System.out.println(currentTime2.getTime() - currentTime1.getTime());

			System.out.println("begin to process SPARQL query...");
			str = br1.readLine();
			ArrayList<String> tpList = ParseSPARQL(str);

			TreeSet<VariableInfo> VariableInfoSet = new TreeSet<VariableInfo>();
			int curSubjectID = 0, curObjectID = 0, curPredicateID = 0, tmp_direction = 0, variableNum = 0;
			String curVarStr = "";

			TreeMap<String, ArrayList<String>> QueryAdjacentList = new TreeMap<String, ArrayList<String>>();
			TreeMap<String, ArrayList<NeighborInfo>> QueryStarPatternMap = new TreeMap<String, ArrayList<NeighborInfo>>();
			ArrayList<String> curList = null;
			HashSet<String> varSet = new HashSet<String>();

			for (int tpIdx = 0; tpIdx < tpList.size(); tpIdx++) {
				str = tpList.get(tpIdx);
				TermArr = str.split("\t");

				curPredicateID = PredicateIDMap.get(TermArr[1]);
				tmp_direction = -1;

				if (!(TermArr[0].startsWith("?"))) {
					curSubjectID = EntityIDMap.get(TermArr[0]);
					TermArr2 = adjacentList[curSubjectID];
					tmp_direction = 0;
					curVarStr = TermArr[2];
				} else {
					varSet.add(TermArr[0]);
				}

				if (!(TermArr[2].startsWith("?"))) {
					curObjectID = EntityIDMap.get(TermArr[2]);
					TermArr2 = adjacentList[curObjectID];
					tmp_direction = 1;
					curVarStr = TermArr[0];
				} else {
					varSet.add(TermArr[2]);
				}

				if (tmp_direction == -1) {

					if (!QueryStarPatternMap.containsKey(TermArr[0])) {
						QueryStarPatternMap.put(TermArr[0], new ArrayList<NeighborInfo>());
					}
					NeighborInfo curNeighborInfo1 = new NeighborInfo(TermArr[2].hashCode(), curPredicateID);
					curNeighborInfo1.Direction = 0;
					QueryStarPatternMap.get(TermArr[0]).add(curNeighborInfo1);

					if (!QueryStarPatternMap.containsKey(TermArr[2])) {
						QueryStarPatternMap.put(TermArr[2], new ArrayList<NeighborInfo>());
					}
					NeighborInfo curNeighborInfo = new NeighborInfo(TermArr[0].hashCode(), curPredicateID);
					curNeighborInfo.Direction = 1;
					QueryStarPatternMap.get(TermArr[2]).add(curNeighborInfo);

					if (QueryAdjacentList.containsKey(TermArr[0])) {
						curList = QueryAdjacentList.get(TermArr[0]);
					} else {
						curList = new ArrayList<String>();
					}

					curList.add("0\t" + curPredicateID + "\t" + TermArr[2]);
					QueryAdjacentList.put(TermArr[0], curList);

					if (QueryAdjacentList.containsKey(TermArr[2])) {
						curList = QueryAdjacentList.get(TermArr[2]);
					} else {
						curList = new ArrayList<String>();
					}

					curList.add("1\t" + curPredicateID + "\t" + TermArr[0]);
					QueryAdjacentList.put(TermArr[2], curList);

					str = br1.readLine();
					continue;
				}

				VariableInfo curVariableInfo = new VariableInfo(curVarStr);
				for (int i = 0; i < TermArr2.length; i++) {
					if (TermArr2[i].Direction == tmp_direction) {
						curVariableInfo.addMatches(TermArr2[i].NeighborID);
					}
				}

				if (VariableInfoSet.contains(curVariableInfo)) {
					VariableInfo tempVariableInfo = VariableInfoSet.floor(curVariableInfo);
					tempVariableInfo.retain(curVariableInfo);
				} else {
					VariableInfoSet.add(curVariableInfo);
				}

				str = br1.readLine();
			}

			variableNum = varSet.size();

			TreeMap<String, TreeSet<String>> varPatternMap = generateVarPatternMap(QueryStarPatternMap, ItemFPListMap,
					fpList, LabelItemPosMap);

			Iterator<Entry<String, TreeSet<String>>> iter_var = varPatternMap.entrySet().iterator();
			while (iter_var.hasNext()) {
				Entry<String, TreeSet<String>> e = iter_var.next();
				curVarStr = e.getKey();
				TreeSet<String> aList = e.getValue();

				Iterator<String> iter_fp = aList.iterator();
				while (iter_fp.hasNext()) {
					fpStr = iter_fp.next() + "\t";

					DatabaseEntry theKey = new DatabaseEntry(fpStr.getBytes("UTF-8"));
					DatabaseEntry theData = new DatabaseEntry();

					if (myDatabase2.get(null, theKey, theData, LockMode.DEFAULT) == OperationStatus.SUCCESS) {
						byte[] retData = theData.getData();
						int[] idList = byteToInt2(retData);

						VariableInfo curVariableInfo = new VariableInfo(curVarStr);
						for (int i = 0; i < idList.length; i++) {
							curVariableInfo.addMatches(idList[i]);
						}

						if (VariableInfoSet.contains(curVariableInfo)) {
							VariableInfo tempVariableInfo = VariableInfoSet.floor(curVariableInfo);
							tempVariableInfo.retain(curVariableInfo);
						} else {
							VariableInfoSet.add(curVariableInfo);
						}
					} else {
						System.out.println("No record found for key '" + fpStr + "'.");
					}
				}
			}

			System.out.print("candidates number: ");

			Iterator<VariableInfo> iter2 = VariableInfoSet.iterator();
			int min = Integer.MAX_VALUE;
			VariableInfo minVariableInfo = null;
			while (iter2.hasNext()) {
				VariableInfo tempVariableInfo = iter2.next();
				System.out.print(tempVariableInfo.VariableStr + " " + tempVariableInfo.size() + ", ");
				if (min > tempVariableInfo.size()) {
					minVariableInfo = tempVariableInfo;
					min = tempVariableInfo.size();
				}
			}
			System.out.println();
			System.out.println("minVariableInfo.VariableStr = " + minVariableInfo.VariableStr);

			// finding the variable with the highest selectivity and query plan
			int cur_match_pos = 0;
			String[] variableArr = new String[variableNum];
			for (int i = 0; i < variableNum; i++) {
				variableArr[i] = "";
			}

			ArrayList<String> queryEdgeList = new ArrayList<String>();
			LinkedList<String> queryQueue = new LinkedList<String>();
			variableArr[0] = minVariableInfo.VariableStr;
			queryQueue.add(minVariableInfo.VariableStr);
			String curVariableStr = "";
			String curEdgeStr = "";

			while (queryQueue.size() != 0) {
				curVariableStr = queryQueue.pop();
				if (QueryAdjacentList.containsKey(curVariableStr)) {
					curList = QueryAdjacentList.get(curVariableStr);
					for (int i = 0; i < curList.size(); i++) {
						curEdgeStr = curList.get(i);
						TermArr = curEdgeStr.split("\t");
						cur_match_pos = SearchInArray(variableArr, TermArr[2]);
						if (cur_match_pos != -1) {
							queryQueue.add(TermArr[2]);
							variableArr[cur_match_pos] = TermArr[2];
						}
						if (!containsEdge(queryEdgeList, curVariableStr, TermArr)) {
							queryEdgeList.add(curVariableStr + "\t" + curEdgeStr);
						}
					}
				}
			}

			TreeSet<ResultInfo> resultSet = new TreeSet<ResultInfo>();
			ResultInfo tempResultInfo = null;
			ArrayList<ResultInfo> tempResultInfoList = new ArrayList<ResultInfo>();
			Iterator<Integer> iter1 = minVariableInfo.matchesSet.iterator();

			while (iter1.hasNext()) {
				tempResultInfo = new ResultInfo(variableNum, 0, keywordArr.length);
				tempResultInfo.matchArr[0] = iter1.next();

				if (QueryStarPatternMap.containsKey(variableArr[0])) {
					for (int j = 0; j < keywordArr.length; j++) {
						tempResultInfo.scoreArr[j] = candidateDist[j][tempResultInfo.matchArr[0]];
					}
				}

				tempResultInfoList.add(tempResultInfo);
				if (queryEdgeList.size() == 0)
					resultSet.add(tempResultInfo);
			}

			Collections.sort(tempResultInfoList);
			LinkedList<ResultInfo> resultStack = new LinkedList<ResultInfo>(tempResultInfoList);

			int next_match_pos = 0, cur_socre_bound = Integer.MAX_VALUE;
			while (resultStack.size() != 0 && queryEdgeList.size() != 0) {
				if (resultSet.size() > TopKNum)
					cur_socre_bound = resultSet.last().getScore();

				tempResultInfo = resultStack.pop();
				if (cur_socre_bound != Integer.MAX_VALUE) {
					if (tempResultInfo.getScore() > cur_socre_bound + 1)
						break;
				}

				curEdgeStr = queryEdgeList.get(tempResultInfo.pos);
				TermArr = curEdgeStr.split("\t");
				curPredicateID = Integer.valueOf(TermArr[2]);
				cur_match_pos = findInArrays(variableArr, TermArr[0]);
				next_match_pos = findInArrays(variableArr, TermArr[3]);
				tmp_direction = Integer.valueOf(TermArr[1]);

				TermArr2 = adjacentList[(tempResultInfo.matchArr[cur_match_pos])];

				for (int i1 = 0; i1 < TermArr2.length; i1++) {
					if (TermArr2[i1].Label != curPredicateID || TermArr2[i1].Direction != tmp_direction)
						continue;

					cur_id = TermArr2[i1].NeighborID;
					if (tempResultInfo.contains(cur_id))
						continue;

					VariableInfo curVariableInfo = new VariableInfo(TermArr[3]);
					if (VariableInfoSet.contains(curVariableInfo)) {
						VariableInfo tempVariableInfo = VariableInfoSet.floor(curVariableInfo);
						if (!tempVariableInfo.matchesSet.contains(cur_id))
							continue;
					}

					if (tempResultInfo.matchArr[next_match_pos] == -1) {
						ResultInfo curResultInfo = new ResultInfo(tempResultInfo, tempResultInfo.pos + 1);
						curResultInfo.matchArr[next_match_pos] = cur_id;

						if (QueryStarPatternMap.containsKey(variableArr[next_match_pos])) {
							for (int i = 0; i < keywordArr.length; i++) {
								int temp_dist = candidateDist[i][cur_id];
								if (temp_dist < curResultInfo.scoreArr[i])
									curResultInfo.scoreArr[i] = candidateDist[i][cur_id];
							}
						}

						if (curResultInfo.pos != queryEdgeList.size()) {
							resultStack.push(curResultInfo);
						} else {
							if (curResultInfo.getScore() < cur_socre_bound || cur_socre_bound == Integer.MAX_VALUE)
								resultSet.add(curResultInfo);
							if (resultSet.size() > TopKNum)
								resultSet.pollLast();
						}
					} else {
						if (tempResultInfo.matchArr[next_match_pos] == cur_id) {
							ResultInfo curResultInfo = new ResultInfo(tempResultInfo, tempResultInfo.pos + 1);
							if (curResultInfo.pos != queryEdgeList.size()) {
								resultStack.push(curResultInfo);
							} else {
								if (curResultInfo.getScore() < cur_socre_bound || cur_socre_bound == Integer.MAX_VALUE)
									resultSet.add(curResultInfo);
								if (resultSet.size() > TopKNum)
									resultSet.pollLast();
							}
						}
					}
				}
			}
			br1.close();

			System.out.print("sparql time:");
			Date currentTime3 = new Date();
			System.out.println(currentTime3.getTime() - currentTime2.getTime());

			System.out.print("total time:");
			System.out.println(currentTime3.getTime() - currentTime1.getTime());

			PrintStream out_summary = new PrintStream(new File(dir_index + "/queryAns.txt"));
			PrintStream out_summary_json = new PrintStream(new File(dir_index + "/queryAns.json"));
			out_summary_json.print("{\r\n" + "  \"head\": {\r\n" + "    \"vars\": [");

			for (int i = 0; i < variableArr.length; i++) {
				System.out.print(variableArr[i] + "\t");
				out_summary_json.print("\"" + variableArr[i] + "\"");
				if (i < variableArr.length - 1) {
					out_summary_json.print(",");
				}
			}
			out_summary_json.print("]\r\n" + "  } ,\r\n" + "  \"results\": {\r\n" + "    \"bindings\": [");

			System.out.println();
			System.out.println(resultSet.size());

			Iterator<ResultInfo> iter = resultSet.iterator();
			while (iter.hasNext()) {
				ResultInfo e = iter.next();
				out_summary_json.print("{");
				for (int i = 0; i < e.matchArr.length; i++) {
					out_summary_json.print("\"" + variableArr[i] + "\": { \"type\": ");
					if (IDEntityMap.get(e.matchArr[i]).charAt(0) == '<') {
						out_summary_json.print("\"uri\" , \"value\": ");
						out_summary_json.print("\"" + IDEntityMap.get(e.matchArr[i]) + "\"" + "}");
					} else {
						out_summary_json.print("\"literal\" , \"value\": ");
						out_summary_json.print(IDEntityMap.get(e.matchArr[i]) + "}");
					}
					System.out.print(IDEntityMap.get(e.matchArr[i]) + "\t");
					out_summary.print(IDEntityMap.get(e.matchArr[i]) + "\t");
					if (i < e.matchArr.length - 1) {
						out_summary_json.print(",");
					}
				}
				System.out.println(Arrays.toString(e.scoreArr));
				out_summary.println();
				out_summary_json.print("}");
				if (iter.hasNext()) {
					out_summary_json.print(",");
				}
			}
			out_summary_json.print("]\r\n" + "  }\r\n" + "}");

			System.out.println("keyword count:\t" + keyword_count);
			System.out.println("results can be found at index/queryAns.txt and index/queryAns.json");
			System.out.println("finished!");

			out_summary.flush();
			out_summary.close();
			out_summary_json.flush();
			out_summary_json.close();

			if (myDatabase1 != null) {
				myDatabase1.close();
			}
			if (myDbEnvironment1 != null) {
				myDbEnvironment1.close();
			}
			if (myDatabase2 != null) {
				myDatabase2.close();
			}
			if (myDbEnvironment2 != null) {
				myDbEnvironment2.close();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static TreeMap<String, TreeSet<String>> generateVarPatternMap(
			TreeMap<String, ArrayList<NeighborInfo>> QueryStarPatternMap,
			TreeMap<Integer, ArrayList<Integer>> ItemFPListMap, ArrayList<FPInfo> fpList,
			TreeMap<Integer, Integer> LabelItemPosMap) {

		TreeMap<String, TreeSet<String>> VarPatternMap = new TreeMap<String, TreeSet<String>>();
		Iterator<Entry<String, ArrayList<NeighborInfo>>> iter_var = QueryStarPatternMap.entrySet().iterator();

		int label_count = 0;
		while (iter_var.hasNext()) {
			Entry<String, ArrayList<NeighborInfo>> e = iter_var.next();
			String curVarStr = e.getKey();
			ArrayList<NeighborInfo> curAdjacentList = e.getValue();

			Collections.sort(curAdjacentList);
			VarPatternMap.put(curVarStr, new TreeSet<String>());

			label_count = 1;
			for (int i = 0; i < curAdjacentList.size(); i++) {
				if (i > 0) {
					if (curAdjacentList.get(i).Label != curAdjacentList.get(i - 1).Label
							|| curAdjacentList.get(i).Direction != curAdjacentList.get(i - 1).Direction) {

						int tmp_item = LabelItemPosMap.get(curAdjacentList.get(i - 1).Label);
						if (label_count >= 1) {
							if (curAdjacentList.get(i - 1).Direction == 0) {
								if (ItemFPListMap.containsKey(tmp_item)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
									}
								}
							} else {
								if (ItemFPListMap.containsKey(tmp_item + 3)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 3);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
									}
								}
							}
						}
						if (label_count >= 2) {
							if (curAdjacentList.get(i - 1).Direction == 0) {
								if (ItemFPListMap.containsKey(tmp_item + 1)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 1);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
									}
								}
							} else {
								if (ItemFPListMap.containsKey(tmp_item + 4)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 4);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
									}
								}
							}
						}
						if (label_count >= 3) {
							if (curAdjacentList.get(i - 1).Direction == 0) {
								if (ItemFPListMap.containsKey(tmp_item + 2)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 2);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
									}
								}
							} else {
								if (ItemFPListMap.containsKey(tmp_item + 5)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 5);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
									}
								}
							}
						}

						label_count = 1;
					} else {
						label_count++;
					}
				}
			}

			int tmp_item = LabelItemPosMap.get(curAdjacentList.get(curAdjacentList.size() - 1).Label);
			if (label_count >= 1) {
				if (curAdjacentList.get(curAdjacentList.size() - 1).Direction == 0) {
					if (ItemFPListMap.containsKey(tmp_item)) {
						ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item);
						for (int k = 0; k < tmp_pos_list.size(); k++) {
							VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
						}
					}
				} else {
					if (ItemFPListMap.containsKey(tmp_item + 3)) {
						ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 3);
						for (int k = 0; k < tmp_pos_list.size(); k++) {
							VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
						}
					}
				}
			}
			if (label_count >= 2) {
				if (curAdjacentList.get(curAdjacentList.size() - 1).Direction == 0) {
					if (ItemFPListMap.containsKey(tmp_item + 1)) {
						ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 1);
						for (int k = 0; k < tmp_pos_list.size(); k++) {
							VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
						}
					}
				} else {
					if (ItemFPListMap.containsKey(tmp_item + 4)) {
						ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 4);
						for (int k = 0; k < tmp_pos_list.size(); k++) {
							VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
						}
					}
				}
			}
			if (label_count >= 3) {
				if (curAdjacentList.get(curAdjacentList.size() - 1).Direction == 0) {
					if (ItemFPListMap.containsKey(tmp_item + 2)) {
						ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 2);
						for (int k = 0; k < tmp_pos_list.size(); k++) {
							VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
						}
					}
				} else {
					if (ItemFPListMap.containsKey(tmp_item + 5)) {
						ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 5);
						for (int k = 0; k < tmp_pos_list.size(); k++) {
							VarPatternMap.get(curVarStr).add(fpList.get(tmp_pos_list.get(k)).getFPStr());
						}
					}
				}
			}
		}
		return VarPatternMap;
	}

	private static int findInArrays(String[] variableArr, String str) {
		for (int i = 0; i < variableArr.length; i++) {
			if (variableArr[i].equals(str)) {
				return i;
			}
		}
		return -1;
	}

	private static boolean containsEdge(ArrayList<String> res_edge_list, String curVariableStr, String[] termArr) {
		String str = "";

		for (int i = 0; i < res_edge_list.size(); i++) {
			str = res_edge_list.get(i);
			String[] curTermArr = str.split("\t");

			if (curTermArr[0].equals(termArr[2]) && curTermArr[3].equals(curVariableStr)
					&& curTermArr[2].equals(termArr[1])) {
				return true;
			}
		}
		return false;
	}

	private static int SearchInArray(String[] variableArr, String str) {
		for (int i = 0; i < variableArr.length; i++) {
			if (variableArr[i].equals(str)) {
				return -1;
			}
			if (variableArr[i].equals("")) {
				return i;
			}
		}
		return -2;
	}

	private static int[] byteToInt2(byte[] b) {
		int mask = 0xff, temp = 0;
		int[] n = new int[b.length / 4];

		for (int j = 0; j < n.length; j++) {
			int offset = j + j + j + j;
			for (int i = 0; i < 4; i++) {
				n[j] <<= 8;
				temp = b[offset + i] & mask;
				n[j] |= temp;
			}
		}

		return n;
	}

	private static ArrayList<String> ParseSPARQL(String queryString) {
		ArrayList<String> tpList = new ArrayList<String>();

		try {
			SPARQLParser parser = new SPARQLParser();
			ParsedQuery query = parser.parseQuery(queryString, null);

			StatementPatternCollector collector = new StatementPatternCollector();
			query.getTupleExpr().visit(collector);

			List<StatementPattern> patterns = collector.getStatementPatterns();

			for (int i = 0; i < patterns.size(); i++) {
				StatementPattern curPattern = patterns.get(i);
				tpList.add(TP2String(curPattern));
			}

		} catch (Exception e) {
			e.printStackTrace();
		}

		return tpList;
	}

	private static String TP2String(StatementPattern curPattern) {
		String curTriplePatternStr = "";

		if (!curPattern.getSubjectVar().isConstant()) {
			curTriplePatternStr += "?" + curPattern.getSubjectVar().getName() + "\t";
		} else {
			curTriplePatternStr += "<" + curPattern.getSubjectVar().getValue().toString() + ">\t";
		}

		if (!curPattern.getPredicateVar().isConstant()) {
			curTriplePatternStr += "?" + curPattern.getPredicateVar().getName() + "\t";
		} else {
			curTriplePatternStr += "<" + curPattern.getPredicateVar().getValue().toString() + ">\t";
		}

		if (!curPattern.getObjectVar().isConstant()) {
			curTriplePatternStr += "?" + curPattern.getObjectVar().getName() + "\t";
		} else {
			if (!curPattern.getObjectVar().getValue().toString().startsWith("\"")) {
				curTriplePatternStr += "<" + curPattern.getObjectVar().getValue().toString() + ">\t";
			} else {
				curTriplePatternStr += curPattern.getObjectVar().getValue().toString();
			}
		}
		curTriplePatternStr = curTriplePatternStr.replace("^^<http://www.w3.org/2001/XMLSchema#string>", "");
		curTriplePatternStr = curTriplePatternStr.trim();

		return curTriplePatternStr;
	}
}
