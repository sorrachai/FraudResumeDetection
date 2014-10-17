package experiments;


public class Item {
  private String date; 
  private String mode;
  private String unit;
  private String current;
  private String interactive;
  
  private String resumeID;
  private String structuredXMLResume;
  private String userArea;
  
  public String getresumeID() {
	    return resumeID;
  }
  public void setresumeID(String _resumeID) {
	    this.resumeID = _resumeID;
  }
  public String getstructuredXMLResume() {
	    return structuredXMLResume;
  }
  public void setstructuredXMLResume(String _structuredXMLResume) {
	    this.structuredXMLResume = _structuredXMLResume;
  }
  public String getuserArea() {
	    return userArea;
  }
  public void setuserArea(String _userArea) {
	    this.userArea = _userArea;
  }
  
  
  public String getDate() {
    return date;
  }
  public void setDate(String date) {
    this.date = date;
  }
  public String getMode() {
    return mode;
  }
  public void setMode(String mode) {
    this.mode = mode;
  }
  public String getUnit() {
    return unit;
  }
  public void setUnit(String unit) {
    this.unit = unit;
  }
  public String getCurrent() {
    return current;
  }
  public void setCurrent(String current) {
    this.current = current;
  }
  public String getInteractive() {
    return interactive;
  }
  public void setInteractive(String interactive) {
    this.interactive = interactive;
  }

  @Override
  public String toString() {
    return "Item [current=" + current + ", date=" + date + ", interactive="
        + interactive + ", mode=" + mode + ", unit=" + unit + "]";
  }
} 