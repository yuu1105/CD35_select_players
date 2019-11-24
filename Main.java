package analysisProcon2019;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

public class Main {
	enum FirstSecond{
		FIRST,
		SECOND
	}
	enum WinLose{
		WIN,
		EVEN,
		LOSE;
		public static WinLose toEnum(String str)
		{
			if( "win".equals(str)) {
				return WIN;
			}
			else if("lose".equals(str))
			{
				return LOSE;
			}
			else
			{
				return EVEN;
			}
		}
	}

	public static class PitcherInfo{
		public String name;
		public Integer toukyuukaisuu=0;
		public Integer sutten=0;
	}
	public static class BatterInfo{
		public String name;
		public Integer dasekisuu = 0;
		public Integer sikyuu = 0;
		public Integer andasuu = 0;
		public Integer andasuu1 = 0;
		public Integer andasuu2 = 0;
		public Integer andasuu3 = 0;
		public Integer andasuu4 = 0;
		public Integer daten = 0;
	}
	public static class MatchResult{
		public FirstSecond fOrS;
		public WinLose result;
		public TeamInfo opponent;
		public Integer soutokuten = 0;
		public Integer sousitten = 0;
		public Integer[] tokuten = new Integer[12];
		public Integer[] sitten = new Integer[12];
		public HashMap<String, BatterInfo> batter = new HashMap<String,BatterInfo>();
		public PitcherInfo pitcher = new PitcherInfo();

	}
	public static class TeamInfo{
		public String name;
		public Integer[] batters = new Integer[9];
		public Integer pitcher;
		public String fileName;
		public ArrayList<MatchResult> match = new ArrayList<MatchResult>();

		public Integer soutokuten = 0;
		public Integer sousitten = 0;
		public Integer dasekisuu = 0;
		public Integer andasuu = 0;
		public Integer andasuu1 = 0;
		public Integer andasuu2 = 0;
		public Integer andasuu3 = 0;
		public Integer andasuu4 = 0;
		public Integer sikyuusuu = 0;
		public Integer toukyuukaisuu = 0;
		public Integer daten = 0;
		public Integer win = 0;
		public Integer lose = 0;
		public Integer even = 0;

		public String toString()
		{
			String str = "[" + name + "]\n";
			for( Integer b : batters)
			{
				str += ""+b+"\n";
			}
			str += ""+pitcher;

			return str;
		}
	}
	private Map<String, TeamInfo> teamInfos = new HashMap<String, TeamInfo>();

	public Map<String, TeamInfo> getTeamInfos() {
		return teamInfos;
	}

	private static final String DIR_PATH = "C:\\Users\\xhfkr\\Downloads\\試合結果詳細\\Open\\League04";
	private static final String TEAM_LIST_FILE_NAME = "TeamList.txt";
	private static final String SEASON_RESULT_FILE_NAME = "season_result.txt";
	private static final Integer INVALID_POINT = -99;

	public void readTeamInfo() {
		ArrayList<String> playerFileNameList = new ArrayList<String>();

		HashMap< String , File> teamResultFileDir = new HashMap< String , File>();
		// TeamList読み込む
		{
			String teamListPath = DIR_PATH + "/" + TEAM_LIST_FILE_NAME;


			BufferedReader r;
			try {

				r = new BufferedReader(new FileReader(teamListPath));
				String data;
		        while ((data = r.readLine()) != null) {
		            System.out.println(data);

		            playerFileNameList.add(data.substring(2));
		        }
		        r.close();
			} catch (FileNotFoundException e) {
				// TODO 自動生成された catch ブロック
				e.printStackTrace();
			} catch (IOException e) {
				// TODO 自動生成された catch ブロック
				e.printStackTrace();
			} finally {

			}
		}
		// 各々のチーム情報読み込む
		for( String playerFileName : playerFileNameList)
		{
			String teamInfoPath = DIR_PATH + "/" + playerFileName;
			TeamInfo teamInfo = new TeamInfo();
			BufferedReader r;
			try {

				r = new BufferedReader(new FileReader(teamInfoPath));
				String data;

				// チーム名
				{
					data = r.readLine();
					teamInfo.name = data;
				}

				// バッター
				for( int i = 0 ; i < 9 ; i++ )
				{
					data = r.readLine();
					teamInfo.batters[i] = Integer.valueOf(data.split("#")[0].trim());
				}

				// ピッチャー
				data = r.readLine();
				teamInfo.pitcher = Integer.valueOf(data.split("#")[0].trim());
				System.out.println(teamInfo);
		        r.close();
			} catch (FileNotFoundException e) {
				// TODO 自動生成された catch ブロック
				e.printStackTrace();
			} catch (IOException e) {
				// TODO 自動生成された catch ブロック
				e.printStackTrace();
			} finally {

			}
			teamInfo.fileName = playerFileName;
			teamInfos.put(teamInfo.name,teamInfo );

			// 結果詳細をコピーするディレクトリを作成する。
			File f = new File(DIR_PATH + "/" + teamInfo.name +"_result" );
			if(!f.exists())
			{
				f.mkdir();
			}
			teamResultFileDir.put(teamInfo.name, f);
		}

		// 試合詳細結果取得
		{
			File[] disResultFiles = new File(DIR_PATH).listFiles( new FilenameFilter(){
				@Override
				public boolean accept(File dir, String name) {
					if (name.endsWith("txt")){
						if( !TEAM_LIST_FILE_NAME.equals(name) && !SEASON_RESULT_FILE_NAME.contentEquals(name))
						return true;
					}

					return false;
				}
			});

			for(File f : disResultFiles)
			{
				// System.out.println(f);

				try {
					MatchResult res1 = new MatchResult();
					MatchResult res2 = new MatchResult();
					res1.fOrS = FirstSecond.FIRST;

					BufferedReader r = new BufferedReader(new FileReader(f));
					String s1 = r.readLine();
					s1 = s1.substring(1, s1.length() - 1);
					TeamInfo ti1 = teamInfos.get(s1);

					Path sourcePath = Paths.get(f.getPath());
		            Path targetPath = Paths.get(teamResultFileDir.get(s1).getPath()+"\\"+f.getName());
		            Files.copy(sourcePath, targetPath, StandardCopyOption.REPLACE_EXISTING);

					res2.opponent = ti1;
					for(int i = 0 ; i < 9 ; i++)
					{
						String buff = r.readLine();
						res1.batter.put(buff.substring(5,buff.indexOf(",")), new BatterInfo());
					}
					String buff1 = r.readLine();
					res1.pitcher.name = buff1.substring(5,buff1.indexOf(","));

					r.readLine();

					String s2 = r.readLine();
					s2 = s2.substring(1, s2.length() - 1);
					TeamInfo ti2 = teamInfos.get(s2);

					Path targetPath2 = Paths.get(teamResultFileDir.get(s2).getPath()+"\\"+f.getName());
		            Files.copy(sourcePath, targetPath2, StandardCopyOption.REPLACE_EXISTING);

					res1.opponent = ti2;
					for(int i = 0 ; i < 9 ; i++)
					{
						String buff = r.readLine();
						res2.batter.put(buff.substring(5,buff.indexOf(",")), new BatterInfo());
					}
					String buff2 = r.readLine();
					res2.pitcher.name = buff2.substring(5,buff2.indexOf(","));


					r.readLine();
					r.readLine();

					res1.result = WinLose.toEnum(r.readLine());
					if(WinLose.WIN ==  res1.result)
					{
						ti1.win++;
					}
					else if(WinLose.EVEN == res1.result)
					{
						ti1.even++;
					}
					else
					{
						ti1.lose++;
					}

					r.readLine();

					res2.result = WinLose.toEnum(r.readLine());
					if(WinLose.WIN ==  res2.result)
					{
						ti2.win++;
					}
					else if(WinLose.EVEN == res2.result)
					{
						ti2.even++;
					}
					else
					{
						ti2.lose++;
					}

					// スコア
					String s3 = r.readLine();
					res1.soutokuten = Integer.valueOf(s3.substring(s3.indexOf(">") + 1, s3.indexOf("-")).trim());
					res2.sousitten = res1.soutokuten;

					res2.soutokuten = Integer.valueOf(s3.substring(s3.indexOf("-") + 1, s3.lastIndexOf("<")).trim());
					res1.sousitten = res2.soutokuten;

					r.readLine();

					String buff3 = r.readLine();
					for( int i = 0 ; i < 12 ; i++ )
					{
						String str = buff3.substring(i*4, 4+i*4).trim();
						if( "x".equals(str)) {
							res1.tokuten[i] = INVALID_POINT;
							res2.sitten[i] = INVALID_POINT;
						}else {
							res1.tokuten[i] = Integer.valueOf(str);
							res2.sitten[i] = Integer.valueOf(str);
							res2.pitcher.toukyuukaisuu++;
						}
					}

					r.readLine();

					String buff4 = r.readLine();
					for( int i = 0 ; i < 12 ; i++ )
					{
						String str = buff4.substring(i*4, 4+i*4).trim();
						if( "x".equals(str)) {
							res2.tokuten[i] = INVALID_POINT;
							res1.sitten[i] = INVALID_POINT;
						}else {
							res2.tokuten[i] = Integer.valueOf(str);
							res1.sitten[i] = Integer.valueOf(str);
							res1.pitcher.toukyuukaisuu++;
						}
					}

					r.readLine();
					r.readLine();
					{
						String data;
						int kai = 0;
						FirstSecond fs = FirstSecond.FIRST;

						while (( data = r.readLine()) != null) {
							if("".equals(data.trim()))
							{
								// next
								if(FirstSecond.FIRST == fs ) {
									fs = FirstSecond.SECOND;
								}else {
									fs = FirstSecond.FIRST;
									kai++;
								}

								continue;
							}

							MatchResult resKougeki;
							MatchResult resSyubi;
							if(fs == FirstSecond.FIRST) {
								resKougeki = res1;
								resSyubi = res2;
							}else {
								resKougeki = res2;
								resSyubi = res1;
							}


							String dasekiKekka = data.substring(5,8).trim();
							String daten = data.substring(10,11).trim();
							String dasyaName;
							if(!"".equals(daten))
							{
								dasyaName = data.substring(13);
							}else {
								dasyaName = data.substring(14);
							}

							BatterInfo b = resKougeki.batter.get(dasyaName);

							b.dasekisuu++;

							if(!"".equals(daten))
							{
								b.daten += Integer.valueOf(daten);
								resSyubi.pitcher.sutten += Integer.valueOf(daten);
							}

							if("　四球".equals(dasekiKekka)) {
								b.sikyuu++;
							}else if("一塁打".equals(dasekiKekka)){
								b.andasuu++;
								b.andasuu1++;
							}else if("二塁打".equals(dasekiKekka)){
								b.andasuu++;
								b.andasuu2++;
							}else if("三塁打".equals(dasekiKekka)){
								b.andasuu++;
								b.andasuu3++;
							}else if("本塁打".equals(dasekiKekka)){
								b.andasuu++;
								b.andasuu4++;
							}
						}
					}

					ti1.match.add(res1);
					ti1.soutokuten += res1.soutokuten;
					ti1.sousitten += res1.sousitten;
					ti1.toukyuukaisuu += res1.pitcher.toukyuukaisuu;
					for(Map.Entry<String,Main.BatterInfo> bi : res1.batter.entrySet())
					{
						ti1.dasekisuu += bi.getValue().dasekisuu;
						ti1.sikyuusuu += bi.getValue().sikyuu;
						ti1.andasuu += bi.getValue().andasuu;
						ti1.andasuu1 += bi.getValue().andasuu1;
						ti1.andasuu2 += bi.getValue().andasuu2;
						ti1.andasuu3 += bi.getValue().andasuu3;
						ti1.andasuu4 += bi.getValue().andasuu4;
						ti1.daten += bi.getValue().daten;
					}
					ti2.match.add(res2);
					ti2.soutokuten += res2.soutokuten;
					ti2.sousitten += res2.sousitten;
					ti2.toukyuukaisuu += res2.pitcher.toukyuukaisuu;
					for(Map.Entry<String,Main.BatterInfo> bi : res2.batter.entrySet())
					{
						ti2.dasekisuu += bi.getValue().dasekisuu;
						ti2.sikyuusuu += bi.getValue().sikyuu;
						ti2.andasuu += bi.getValue().andasuu;
						ti2.andasuu1 += bi.getValue().andasuu1;
						ti2.andasuu2 += bi.getValue().andasuu2;
						ti2.andasuu3 += bi.getValue().andasuu3;
						ti2.andasuu4 += bi.getValue().andasuu4;
						ti2.daten += bi.getValue().daten;
					}

					r.close();
				} catch (FileNotFoundException e) {
					// TODO 自動生成された catch ブロック
					e.printStackTrace();
				} catch (IOException e) {
					// TODO 自動生成された catch ブロック
					e.printStackTrace();
				}
			}
		}

	}


	public static void main(String[] args) {
		Main obj = new Main();

		obj.readTeamInfo();
		System.out.println("<tr>");
		System.out.println("<th class=\"sort\" data-sort=\"name\">"+"チーム名"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"win\">"+"勝利"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"lose\">"+"敗退"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"even\">"+"引分"+"</th>");
		int vs = 1;
		for(Map.Entry<String, TeamInfo> tmp : obj.getTeamInfos().entrySet())
		{
			System.out.println("<th class=\"sort\" data-sort=\"vs"+vs+"\">"+tmp.getValue().name+"</th>");
			vs++;
		}
		System.out.println("<th class=\"sort\" data-sort=\"soutokuten\">"+"総得点"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"sousitten\">"+"総失点"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"dasekisuu\">"+"打席数"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"andasuu\">"+"安打数"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"andasuu1\">"+"一塁打"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"andasuu2\">"+"二塁打"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"andasuu3\">"+"三塁打"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"andasuu4\">"+"本塁打"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"sikyuusuu\">"+"四球"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"daritu\">"+"打率"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"syuturuiritu\">"+"出塁率"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"tyoudaritu\">"+"長打率"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"ops\">"+"ＯＰＳ"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"daten\">"+"打点"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"toukyuukaisuu\">"+"投球回数"+"</th>");
		System.out.println("<th class=\"sort\" data-sort=\"bougyoritu\">"+"防御率"+"</th>");
		System.out.println("</tr>");

		System.out.println("");
		System.out.println("");
		for(Map.Entry<String, TeamInfo> info : obj.getTeamInfos().entrySet())
		{
			System.out.println("<tr>");
			System.out.println("<td class=\"name\">"+info.getValue().name+"</td>");
			System.out.println("<td class=\"win\">"+info.getValue().win+"</td>");
			System.out.println("<td class=\"lose\">"+info.getValue().lose+"</td>");
			System.out.println("<td class=\"even\">"+info.getValue().even+"</td>");
			vs = 1;
			for(Map.Entry<String, TeamInfo> tmp : obj.getTeamInfos().entrySet())
			{
				System.out.println("<td class=\"vs"+vs+"\">"+" x-x-x "+"</td>");
				vs++;
			}
			System.out.println("<td class=\"soutokuten\">"+info.getValue().soutokuten+"</td>");
			System.out.println("<td class=\"sousitten\">"+info.getValue().sousitten+"</td>");
			System.out.println("<td class=\"dasekisuu\">"+info.getValue().dasekisuu+"</td>");
			System.out.println("<td class=\"andasuu\">"+info.getValue().andasuu+"</td>");
			System.out.println("<td class=\"andasuu1\">"+info.getValue().andasuu1+"</td>");
			System.out.println("<td class=\"andasuu2\">"+info.getValue().andasuu2+"</td>");
			System.out.println("<td class=\"andasuu3\">"+info.getValue().andasuu3+"</td>");
			System.out.println("<td class=\"andasuu4\">"+info.getValue().andasuu4+"</td>");
			System.out.println("<td class=\"sikyuusuu\">"+info.getValue().sikyuusuu+"</td>");
			System.out.printf("<td class=\"daritu\">%.2f</td>\n", ( (double)info.getValue().andasuu/(info.getValue().dasekisuu-info.getValue().sikyuusuu)));
			double syuturuiritu = (double)(info.getValue().andasuu+info.getValue().sikyuusuu)/info.getValue().dasekisuu;
			System.out.printf("<td class=\"syuturuiritu\">%.2f</td>\n", syuturuiritu);
			double tyoudaritu = ((double)info.getValue().andasuu1 + info.getValue().andasuu2 * 2 + info.getValue().andasuu3 * 3 + info.getValue().andasuu4 * 4 ) / (info.getValue().dasekisuu-info.getValue().sikyuusuu);
			System.out.printf("<td class=\"tyoudaritu\">%.2f</td>\n", tyoudaritu);
			System.out.printf("<td class=\"ops\">%.2f</td>\n",syuturuiritu+tyoudaritu);
			System.out.println("<td class=\"daten\">"+info.getValue().daten+"</td>");
			System.out.println("<td class=\"toukyuukaisuu\">"+info.getValue().toukyuukaisuu+"</td>");
			System.out.printf("<td class=\"bougyoritu\">%.2f</td>\n",info.getValue().sousitten*9.0/info.getValue().toukyuukaisuu);
			System.out.println("</tr>");
		}

		System.out.println("");
		System.out.println("");

	}

}
