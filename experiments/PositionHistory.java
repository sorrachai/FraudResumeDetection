package experiments;

public class PositionHistory {
	String positionType;
	String currentEmployer;
	String title;
	OrgName orgName;
	OrgInfo orgInfo;
	String description;
	StartDate startDate;
	EndDate endDate;
	String comments;
	public String getPositionType() {
		return positionType;
	}
	public void setPositionType(String positionType) {
		this.positionType = positionType;
	}
	public String getCurrentEmployer() {
		return currentEmployer;
	}
	public void setCurrentEmployer(String currentEmployer) {
		this.currentEmployer = currentEmployer;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public OrgName getOrgName() {
		return orgName;
	}
	public void setOrgName(OrgName orgName) {
		this.orgName = orgName;
	}
	public OrgInfo getOrgInfo() {
		return orgInfo;
	}
	public void setOrgInfo(OrgInfo orgInfo) {
		this.orgInfo = orgInfo;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public StartDate getStartDate() {
		return startDate;
	}
	public void setStartDate(StartDate startDate) {
		this.startDate = startDate;
	}
	public EndDate getEndDate() {
		return endDate;
	}
	public void setEndDate(EndDate endDate) {
		this.endDate = endDate;
	}
	public String getComments() {
		return comments;
	}
	public void setComments(String comments) {
		this.comments = comments;
	}

}
