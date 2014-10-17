package experiments;

import java.util.List;

import experiments.Item;

public class TestRead {
  public static void main(String args[]) {
    StaXParser read = new StaXParser();
    List<Item> readConfig = read.readConfig("/Users/vaibhavsharma/workspace/FraudResumeDetector/bin/experiments/config.xml");
    for (Item item : readConfig) {
      System.out.println(item);
    }
  }
} 