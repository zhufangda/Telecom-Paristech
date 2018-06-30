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
import java.util.Iterator;
import java.util.Map.Entry;
import java.util.TreeMap;
import java.util.TreeSet;

import org.apache.lucene.analysis.Analyzer;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.index.IndexWriter;

import com.sleepycat.je.Database;
import com.sleepycat.je.DatabaseConfig;
import com.sleepycat.je.DatabaseEntry;
import com.sleepycat.je.Environment;
import com.sleepycat.je.EnvironmentConfig;

import utils.FPInfo;
import utils.ItemInfo;
import utils.KeyValuePair;
import utils.NeighborInfo;
import utils.HeapNode;

public class Indexing {

	static int amplifier = 1000000;

	@SuppressWarnings("deprecation")
	public static void main(String[] args) {
		if (args.length != 3) {
			System.out.println("usage:");
			System.out.println("================");
			System.out.println(
					"java -classpath algo.jar Indexing <data file location> <pivot number> <0/1 if need structrual index>");
			System.out.println("");
			System.out.println("generating structrual index can be very long for huge data!");
			System.out.println("structrual index is only necessary for query answering!");
			System.out.println("");
			System.out.println("for example,");
			System.out.println("java -classpath algo.jar Indexing data/persons.nt 200 0");
			System.exit(0);
		}
		try {
			String str = "";
			String dir_index = "index";
			String[] TermArr;
			int PivotNum = Integer.valueOf(args[1]);

			File file = new File(dir_index);
			if (!file.exists()) {
				file.mkdirs();
			}
			File file1 = new File(dir_index + "/DistanceIndex");
			if (!file1.exists()) {
				file1.mkdirs();
			}
			File file2 = new File(dir_index + "/LuceneIndex");
			if (!file2.exists()) {
				file2.mkdirs();
			}
			File file3 = new File(dir_index + "/StructuralIndex");
			if (!file3.exists()) {
				file3.mkdirs();
			}

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

			InputStream inFile = new FileInputStream(new File(args[0]));
			Reader inReader = new InputStreamReader(inFile);
			BufferedReader inBuffer = new BufferedReader(inReader);

			int head_id = 0, tail_id = 0, label_id = 0, cur_id = 0, NodeNum = 0, p_id = 0;
			String objStr = "";
			ArrayList<ArrayList<NeighborInfo>> adjacentList1 = new ArrayList<ArrayList<NeighborInfo>>();
			ArrayList<NeighborInfo> curList = new ArrayList<NeighborInfo>();
			TreeMap<String, Integer> EntityIDMap = new TreeMap<String, Integer>();
			HashMap<Integer, String> IDEntityMap = new HashMap<Integer, String>();
			TreeMap<String, Integer> PredicateIDMap = new TreeMap<String, Integer>();
			TreeMap<Integer, String> IDPredicateMap = new TreeMap<Integer, String>();
			TreeMap<Integer, TreeSet<Integer>> predicateSalienceForwardMap = new TreeMap<Integer, TreeSet<Integer>>();
			TreeMap<Integer, TreeSet<Integer>> predicateSalienceBackwardMap = new TreeMap<Integer, TreeSet<Integer>>();

			str = inBuffer.readLine();
			System.out.println("load RDF data, genenerate inverted index...");
			while (str != null) {
				str = str.trim();
				str = str.substring(0, str.length() - 1);
				str = str.trim();
				TermArr = str.split(" ");

				objStr = "";
				for (int i = 2; i < TermArr.length; i++) {
					objStr += TermArr[i] + " ";
				}
				objStr = objStr.trim();

				if (!EntityIDMap.containsKey(TermArr[0])) {
					EntityIDMap.put(TermArr[0], cur_id);
					IDEntityMap.put(cur_id, TermArr[0]);
					adjacentList1.add(new ArrayList<NeighborInfo>());
					cur_id++;
				}

				if (!EntityIDMap.containsKey(objStr)) {
					EntityIDMap.put(objStr, cur_id);
					IDEntityMap.put(cur_id, objStr);
					adjacentList1.add(new ArrayList<NeighborInfo>());
					cur_id++;
				}

				if (!PredicateIDMap.containsKey(TermArr[1])) {
					PredicateIDMap.put(TermArr[1], p_id);
					IDPredicateMap.put(p_id, TermArr[1]);
					predicateSalienceForwardMap.put(p_id, new TreeSet<Integer>());
					predicateSalienceBackwardMap.put(p_id, new TreeSet<Integer>());
					p_id++;
				}

				head_id = EntityIDMap.get(TermArr[0]);
				label_id = PredicateIDMap.get(TermArr[1]);
				predicateSalienceForwardMap.get(label_id).add(head_id);

				tail_id = EntityIDMap.get(objStr);
				predicateSalienceBackwardMap.get(label_id).add(tail_id);

				NeighborInfo newNeighborInfo1 = new NeighborInfo(tail_id, label_id);
				newNeighborInfo1.Direction = 0;
				adjacentList1.get(head_id).add(newNeighborInfo1);

				NeighborInfo newNeighborInfo2 = new NeighborInfo(head_id, label_id);
				newNeighborInfo2.Direction = 1;
				adjacentList1.get(tail_id).add(newNeighborInfo2);

				str = inBuffer.readLine();
			}

			NodeNum = cur_id;
			inBuffer.close();

			InputStream in = new FileInputStream(new File(args[0]));
			Reader inr = new InputStreamReader(in);
			BufferedReader br = new BufferedReader(inr);

			String[] entityLiteralArr = new String[NodeNum];
			Arrays.fill(entityLiteralArr, "");

			str = br.readLine();
			while (str != null) {
				str = str.trim();
				str = str.substring(0, str.length() - 1);
				str = str.trim();
				TermArr = str.split(" ");

				objStr = "";
				for (int i = 2; i < TermArr.length; i++) {
					objStr += TermArr[i] + " ";
				}
				objStr = objStr.trim();

				cur_id = EntityIDMap.get(TermArr[0]);
				if (!objStr.startsWith("<")) {
					entityLiteralArr[cur_id] += objStr + "\n";
				}

				str = br.readLine();
			}
			br.close();

			File indexDir = new File(dir_index + "/LuceneIndex");
			Analyzer luceneAnalyzer = new StandardAnalyzer();
			IndexWriter indexWriter = new IndexWriter(indexDir, luceneAnalyzer, true);

			for (cur_id = 0; cur_id < NodeNum; cur_id++) {
				str = entityLiteralArr[cur_id];

				Document document = new Document();

				Field FieldBody = new Field("body", str, Field.Store.YES, Field.Index.TOKENIZED,
						Field.TermVector.WITH_POSITIONS_OFFSETS);

				Field FieldTitle = new Field("title", "" + cur_id, Field.Store.YES, Field.Index.NO);

				document.add(FieldBody);
				document.add(FieldTitle);
				indexWriter.addDocument(document);
			}

			indexWriter.optimize();
			indexWriter.close();

			System.out.println("writing entity_id_map...");
			PrintStream out_entity = new PrintStream(new File(dir_index + "/entity_id_map.txt"));
			Iterator<Entry<String, Integer>> iter32 = EntityIDMap.entrySet().iterator();
			byte[] entityTagArr = new byte[NodeNum];
			Arrays.fill(entityTagArr, (byte) 0);

			while (iter32.hasNext()) {
				Entry<String, Integer> e = iter32.next();
				cur_id = e.getValue();
				str = e.getKey();

				out_entity.println(cur_id + "\t" + str);
				if (str.startsWith("<")) {
					entityTagArr[cur_id] = 1;
				}
			}
			out_entity.flush();
			out_entity.close();

			System.out.println("writing predicate weights...");
			PrintStream out_p_weight = new PrintStream(new File(dir_index + "/p_weight.txt"));
			Iterator<Entry<Integer, TreeSet<Integer>>> iter82 = predicateSalienceBackwardMap.entrySet().iterator();
			int item_id = 0;
			TreeMap<Integer, ItemInfo> IDItemMap = new TreeMap<Integer, ItemInfo>();
			TreeMap<Integer, Integer> LabelItemPosMap = new TreeMap<Integer, Integer>();

			while (iter82.hasNext()) {
				Entry<Integer, TreeSet<Integer>> e = iter82.next();
				p_id = e.getKey();

				TreeSet<Integer> s1 = e.getValue();
				TreeSet<Integer> s2 = predicateSalienceForwardMap.get(p_id);

				s1.addAll(s2);
				double tmp_weight = (s1.size() * 1.0) / (adjacentList1.size());
				int cur_weight = (int) Math.log((tmp_weight * amplifier));
				out_p_weight.println(IDPredicateMap.get(p_id) + " " + p_id + " " + cur_weight);
				predicateSalienceForwardMap.put(p_id, s1);

				if (!LabelItemPosMap.containsKey(p_id)) {
					LabelItemPosMap.put(p_id, item_id);

					IDItemMap.put(item_id, new ItemInfo(p_id, 0, 1));
					IDItemMap.put((item_id + 1), new ItemInfo(p_id, 0, 2));
					IDItemMap.put((item_id + 2), new ItemInfo(p_id, 0, 3));

					IDItemMap.put((item_id + 3), new ItemInfo(p_id, 1, 1));
					IDItemMap.put((item_id + 4), new ItemInfo(p_id, 1, 2));
					IDItemMap.put((item_id + 5), new ItemInfo(p_id, 1, 3));

					item_id = item_id + 6;
				}
			}
			out_p_weight.flush();
			out_p_weight.close();

			System.out.println("writing adjacences and transactions...");
			PrintStream out_adjacent_list = new PrintStream(new File(dir_index + "/graph_adjacent_list.txt"));
			PrintStream out_transaction = new PrintStream(new File(dir_index + "/multi_transaction_set.txt"));

			NeighborInfo[][] adjacentList = new NeighborInfo[NodeNum][];
			int max_item_count = 0;

			out_adjacent_list.println(NodeNum);
			for (cur_id = 0; cur_id < adjacentList1.size(); cur_id++) {

				curList = adjacentList1.get(cur_id);

				Collections.sort(curList);

				adjacentList[cur_id] = new NeighborInfo[curList.size()];

				out_adjacent_list.print(cur_id + "\t");
				ArrayList<Integer> out_transaction_list = new ArrayList<Integer>();
				Iterator<NeighborInfo> iter = curList.iterator();
				int i = 0, label_count = 1;
				while (iter.hasNext()) {
					NeighborInfo curNeighborInfo = iter.next();

					out_adjacent_list.print(curNeighborInfo.NeighborID + " ");
					out_adjacent_list.print(curNeighborInfo.Label + " ");
					double tmp_weight = ((predicateSalienceForwardMap.get(curNeighborInfo.Label).size()) * 1.0)
							/ (adjacentList1.size());
					curNeighborInfo.Distance = (int) Math.log((tmp_weight * amplifier));
					out_adjacent_list.print(curNeighborInfo.Distance + " ");
					out_adjacent_list.print(curNeighborInfo.Direction + " ");
					adjacentList[cur_id][i] = curNeighborInfo;

					if (i > 0) {
						if (adjacentList[cur_id][i].Label != adjacentList[cur_id][i - 1].Label
								|| adjacentList[cur_id][i].Direction != adjacentList[cur_id][i - 1].Direction) {

							int tmp_item = LabelItemPosMap.get(adjacentList[cur_id][i - 1].Label);
							if (label_count >= 1) {
								if (adjacentList[cur_id][i - 1].Direction == 0) {
									out_transaction_list.add(tmp_item + 0);
								} else {
									out_transaction_list.add(tmp_item + 3);
								}
							}
							if (label_count >= 2) {
								if (adjacentList[cur_id][i - 1].Direction == 0) {
									out_transaction_list.add(tmp_item + 1);
								} else {
									out_transaction_list.add(tmp_item + 4);
								}
							}
							if (label_count >= 3) {
								if (adjacentList[cur_id][i - 1].Direction == 0) {
									out_transaction_list.add(tmp_item + 2);
								} else {
									out_transaction_list.add(tmp_item + 5);
								}
							}

							label_count = 1;
						} else {
							label_count++;
						}
					}
					i++;
				}

				int tmp_item = LabelItemPosMap.get(adjacentList[cur_id][adjacentList[cur_id].length - 1].Label);

				if (label_count >= 1) {
					if (adjacentList[cur_id][adjacentList[cur_id].length - 1].Direction == 0) {
						out_transaction_list.add(tmp_item + 0);
					} else {
						out_transaction_list.add(tmp_item + 3);
					}
				}

				if (label_count >= 2) {
					if (adjacentList[cur_id][adjacentList[cur_id].length - 1].Direction == 0) {
						out_transaction_list.add(tmp_item + 1);
					} else {
						out_transaction_list.add(tmp_item + 4);
					}
				}

				if (label_count >= 3) {
					if (adjacentList[cur_id][adjacentList[cur_id].length - 1].Direction == 0) {
						out_transaction_list.add(tmp_item + 2);
					} else {
						out_transaction_list.add(tmp_item + 5);
					}
				}

				Collections.sort(out_transaction_list);
				out_transaction.print(out_transaction_list.size() + " ");
				if (max_item_count < out_transaction_list.size()) {
					max_item_count = out_transaction_list.size();
				}

				str = "";
				for (int j = 0; j < out_transaction_list.size(); j++) {
					str += (out_transaction_list.get(j) + " ");
				}
				str = str.trim();
				out_transaction.println(str);

				out_adjacent_list.println();
			}
			out_adjacent_list.flush();
			out_adjacent_list.close();

			out_transaction.flush();
			out_transaction.close();

			adjacentList1.clear();

			if (Integer.valueOf(args[2]) == 1) {
				System.out.println("genenerate the structural index...");
				Date starIndexStartTime = new Date();
				int support_delta = NodeNum / 100;
				if (NodeNum < 1000) {
					support_delta = NodeNum / 10;
				}
				System.out.println("node number: " + NodeNum);

				Process prc = Runtime.getRuntime()
						.exec("./fp_growth.exe -f " + dir_index + "/multi_transaction_set.txt -o " + dir_index
								+ "/fp.out -s " + support_delta + " -t " + NodeNum + " -i " + IDItemMap.size() + " -w "
								+ max_item_count);

				BufferedReader br_prc = new BufferedReader(new InputStreamReader(prc.getInputStream()));
				String line = null;
				while ((line = br_prc.readLine()) != null) {
					System.out.println(line);
				}

				prc.waitFor();

				System.out.println("parsing fp.out......");
				InputStream in70 = new FileInputStream(new File(dir_index + "/fp.out"));
				Reader inr70 = new InputStreamReader(in70);
				BufferedReader br70 = new BufferedReader(inr70);
				TreeSet<Integer> frequentPropertySet = new TreeSet<Integer>();
				TreeSet<Integer> frequentItemSet = new TreeSet<Integer>();
				TreeMap<Integer, ArrayList<Integer>> ItemFPListMap = new TreeMap<Integer, ArrayList<Integer>>();
				ArrayList<FPInfo> fpList = new ArrayList<FPInfo>();
				int cur_frequency = 0;

				str = br70.readLine();
				while (str != null) {
					TermArr = str.split(":");

					String[] TermArr1 = TermArr[0].split(" ");
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

				int label_count = 0;

				for (int j = 0; j < NodeNum; j++) {

					if (adjacentList[j] != null) {
						label_count = 1;
						for (int i = 0; i < adjacentList[j].length; i++) {
							if (i > 0) {
								if (adjacentList[j][i].Label != adjacentList[j][i - 1].Label
										|| adjacentList[j][i].Direction != adjacentList[j][i - 1].Direction) {

									int tmp_item = LabelItemPosMap.get(adjacentList[j][i - 1].Label);

									if (label_count >= 1) {
										if (adjacentList[j][i - 1].Direction == 0) {
											if (ItemFPListMap.containsKey(tmp_item)) {
												ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item);
												for (int k = 0; k < tmp_pos_list.size(); k++) {
													fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
												}
											}
										} else {
											if (ItemFPListMap.containsKey(tmp_item + 3)) {
												ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 3);
												for (int k = 0; k < tmp_pos_list.size(); k++) {
													fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
												}
											}
										}
									}

									if (label_count >= 2) {
										if (adjacentList[j][i - 1].Direction == 0) {
											if (ItemFPListMap.containsKey(tmp_item + 1)) {
												ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 1);
												for (int k = 0; k < tmp_pos_list.size(); k++) {
													fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
												}
											}
										} else {
											if (ItemFPListMap.containsKey(tmp_item + 4)) {
												ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 4);
												for (int k = 0; k < tmp_pos_list.size(); k++) {
													fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
												}
											}
										}
									}

									if (label_count >= 3) {
										if (adjacentList[j][i - 1].Direction == 0) {
											if (ItemFPListMap.containsKey(tmp_item + 2)) {
												ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 2);
												for (int k = 0; k < tmp_pos_list.size(); k++) {
													fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
												}
											}
										} else {
											if (ItemFPListMap.containsKey(tmp_item + 5)) {
												ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 5);
												for (int k = 0; k < tmp_pos_list.size(); k++) {
													fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
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

						int tmp_item = LabelItemPosMap.get(adjacentList[j][adjacentList[j].length - 1].Label);
						if (label_count >= 1) {
							if (adjacentList[j][adjacentList[j].length - 1].Direction == 0) {
								if (ItemFPListMap.containsKey(tmp_item)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
									}
								}
							} else {
								if (ItemFPListMap.containsKey(tmp_item + 3)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 3);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
									}
								}
							}
						}
						if (label_count >= 2) {
							if (adjacentList[j][adjacentList[j].length - 1].Direction == 0) {
								if (ItemFPListMap.containsKey(tmp_item + 1)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 1);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
									}
								}
							} else {
								if (ItemFPListMap.containsKey(tmp_item + 4)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 4);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
									}
								}
							}
						}
						if (label_count >= 3) {
							if (adjacentList[j][adjacentList[j].length - 1].Direction == 0) {
								if (ItemFPListMap.containsKey(tmp_item + 2)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 2);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
									}
								}
							} else {
								if (ItemFPListMap.containsKey(tmp_item + 5)) {
									ArrayList<Integer> tmp_pos_list = ItemFPListMap.get(tmp_item + 5);
									for (int k = 0; k < tmp_pos_list.size(); k++) {
										fpList.get(tmp_pos_list.get(k)).MatchingList.add(j);
									}
								}
							}
						}
					}
				}

				for (int i = 0; i < fpList.size(); i++) {
					String aKey2 = fpList.get(i).getFPStr() + "\t";
					byte[] aData2 = intArrayToBytes(fpList.get(i).MatchingList);

					DatabaseEntry theKey2 = new DatabaseEntry(aKey2.getBytes("UTF-8"));
					DatabaseEntry theData2 = new DatabaseEntry(aData2);
					myDatabase2.put(null, theKey2, theData2);
				}
				Date starIndexEndTime = new Date();
				System.out.println("structural index takes "
						+ (starIndexEndTime.getTime() - starIndexStartTime.getTime()) + " ms.");
			}

			Date pivotStartTime = new Date();
			System.out.println("genenerate the distance-based index...");

			KeyValuePair[] pivotArr = new KeyValuePair[NodeNum];
			for (int k = 0; k < NodeNum; k++) {
				pivotArr[k] = new KeyValuePair();
				pivotArr[k].key = k;
				pivotArr[k].value = 0;
				if (adjacentList[k] != null) {
					pivotArr[k].value = adjacentList[k].length;
				} else {
					adjacentList[k] = new NeighborInfo[0];
				}
			}
			Arrays.sort(pivotArr);

			byte[] visited = new byte[NodeNum];
			byte[] candidateDist = new byte[NodeNum];
			TreeSet<HeapNode> candidate = new TreeSet<HeapNode>();

			PrintStream out_pivot = new PrintStream(new File(dir_index + "/pivot.txt"));
			for (int j = 0; j < PivotNum; j++) {
				int sourceID = pivotArr[pivotArr.length - 1 - j].key;

				if (entityTagArr[sourceID] == 0) {
					PivotNum++;
					continue;
				}

				out_pivot.println(
						pivotArr[pivotArr.length - 1 - j].key + "\t" + pivotArr[pivotArr.length - 1 - j].value + "\t");
				cur_id = 0;
				int cur_dist = 0;
				for (int i = 0; i < visited.length; i++) {
					visited[i] = -1;
					candidateDist[i] = -1;
				}

				visited[sourceID] = 0;
				for (int i = 0; i < adjacentList[sourceID].length; i++) {
					HeapNode curHeapNode = new HeapNode(adjacentList[sourceID][i].NeighborID);
					curHeapNode.Dist = adjacentList[sourceID][i].Distance;
					candidate.add(curHeapNode);
					candidateDist[curHeapNode.VertexID] = (byte) curHeapNode.Dist;
				}

				while (candidate.size() != 0) {
					HeapNode curHeapNode = candidate.pollFirst();
					visited[curHeapNode.VertexID] = (byte) curHeapNode.Dist;

					for (int i = 0; i < adjacentList[curHeapNode.VertexID].length; i++) {
						if (visited[adjacentList[curHeapNode.VertexID][i].NeighborID] != -1)
							continue;
						if (entityTagArr[adjacentList[curHeapNode.VertexID][i].NeighborID] == 0)
							continue;

						if (candidateDist[adjacentList[curHeapNode.VertexID][i].NeighborID] != -1) {
							cur_dist = candidateDist[adjacentList[curHeapNode.VertexID][i].NeighborID];
							if (adjacentList[curHeapNode.VertexID][i].Distance + curHeapNode.Dist < cur_dist) {
								HeapNode tempHeapNode = new HeapNode(adjacentList[curHeapNode.VertexID][i].NeighborID);
								tempHeapNode.Dist = cur_dist;
								candidate.remove(tempHeapNode);

								HeapNode newHeapNode = new HeapNode(adjacentList[curHeapNode.VertexID][i].NeighborID);
								newHeapNode.Dist = adjacentList[curHeapNode.VertexID][i].Distance + curHeapNode.Dist;
								candidate.add(newHeapNode);

								candidateDist[adjacentList[curHeapNode.VertexID][i].NeighborID] = (byte) (adjacentList[curHeapNode.VertexID][i].Distance
										+ curHeapNode.Dist);
							}
						} else {
							HeapNode newHeapNode = new HeapNode(adjacentList[curHeapNode.VertexID][i].NeighborID);
							newHeapNode.Dist = adjacentList[curHeapNode.VertexID][i].Distance + curHeapNode.Dist;
							candidate.add(newHeapNode);

							candidateDist[adjacentList[curHeapNode.VertexID][i].NeighborID] = (byte) (adjacentList[curHeapNode.VertexID][i].Distance
									+ curHeapNode.Dist);
						}
					}
				}

				String aKey1 = "" + pivotArr[pivotArr.length - 1 - j].key;

				DatabaseEntry theKey1 = new DatabaseEntry(aKey1.getBytes("UTF-8"));
				DatabaseEntry theData1 = new DatabaseEntry(visited);
				myDatabase1.put(null, theKey1, theData1);

			}
			out_pivot.flush();
			out_pivot.close();

			myDatabase1.close();
			myDbEnvironment1.close();
			myDatabase2.close();
			myDbEnvironment2.close();

			Date pivotEndTime = new Date();
			System.out.println(
					"distanced-based index takes " + (pivotEndTime.getTime() - pivotStartTime.getTime()) + " ms.");

			System.out.println("finished!");
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static byte[] intArrayToBytes(ArrayList<Integer> a) {
		int offset = 8, pos = 0, m = 0, size = a.size();
		byte[] b = new byte[size + size + size + size];

		for (int i = 0; i < size; i++) {
			offset = 0;
			pos = i + i + i + i;
			m = a.get(i);
			for (int j = 0; j < 4; j++) {
				b[pos + j] = (byte) (m >> (24 - offset));
				offset += 8;
			}
		}
		return b;
	}
}