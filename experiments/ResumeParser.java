package experiments;
//http://www.vogella.com/tutorials/JavaXML/article.html
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import javax.xml.stream.XMLEventReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamException;
import javax.xml.stream.events.Attribute;
import javax.xml.stream.events.EndElement;
import javax.xml.stream.events.StartElement;
import javax.xml.stream.events.XMLEvent;

import experiments.Item;

public class ResumeParser {
	static final String DATE = "date";
	static final String ITEM = "item";
	static final String MODE = "mode";
	static final String UNIT = "unit";
	static final String CURRENT = "current";
	static final String INTERACTIVE = "interactive";

	static final String RESUMEID = "ResumeID";
	static final String STRUCTUREDXMLRESUME = "StructuredXMLResume";
	static final String USERAREA = "UserArea";

	@SuppressWarnings({ "unchecked", "null" })
	
	public PositionHistory getLastPositionHistory(Resume resume) {
		ArrayList<EmployerOrg> tmp = resume.getEmploymentHistory().getEmployerOrg();
		int len = tmp.size();
		return tmp.get(len-1).getPositionHistory();
	}
	
	public void setLastPositionHistory(Resume resume, PositionHistory positionHistory) {
		ArrayList<EmployerOrg> tmp = resume.getEmploymentHistory().getEmployerOrg();
		int len = tmp.size();
		tmp.get(len-1).setPositionHistory(positionHistory);
	}
	
	public List<Item> readConfig(String configFile) {
		List<Item> items = new ArrayList<Item>();
		try {
			// First, create a new XMLInputFactory
			XMLInputFactory inputFactory = XMLInputFactory.newInstance();
			// Setup a new eventReader
			InputStream in = new FileInputStream(configFile);
			XMLEventReader eventReader = inputFactory.createXMLEventReader(in);
			// read the XML document
			Resume resume = null;

			while (eventReader.hasNext()) {
				XMLEvent event = eventReader.nextEvent();

				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("ResumeID")) {
						continue;
					}
				}

				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("XMLResume")) {
						continue;
					}
				}
				
				if(event.isStartElement()) {
					if(event.asStartElement().getName().getLocalPart().equals("StructuredXMLResume"))
						continue;
				}

				if (event.isStartElement()) {
					StartElement startElement = event.asStartElement();
					// If we have an item element, we create a new item
					//if (startElement.getName().getLocalPart() == ("config")) {
					if (startElement.getName().getLocalPart().equals("Resume")) {
						resume = new Resume();
						// We read the attributes from this tag and add the date
						// attribute to our object
//						Iterator<Attribute> attributes = startElement
//								.getAttributes();
//						while (attributes.hasNext()) {
//							Attribute attribute = attributes.next();
//							if (attribute.getName().toString().equals(DATE)) {
//								item.setDate(attribute.getValue());
//							}
//						}
					}
				}

				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("EmploymentHistory")) {
						resume.setEmploymentHistory(new EmploymentHistory());
						event = eventReader.nextEvent();
						//item.setMode(event.asCharacters().getData());
						continue;
					}
				}
				
				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("EmployerOrg")) {
						ArrayList<EmployerOrg> tmp = resume.getEmploymentHistory().getEmployerOrg();
						tmp.add(new EmployerOrg());
						resume.getEmploymentHistory().setEmployerOrg(tmp);
						event = eventReader.nextEvent();
						//item.setMode(event.asCharacters().getData());
						continue;
					}
				}
				
				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("EmployerOrgName")) {
						ArrayList<EmployerOrg> tmp = resume.getEmploymentHistory().getEmployerOrg();
						int len = tmp.size();
						event = eventReader.nextEvent();
						tmp.get(len-1).setEmployerOrgName(event.asCharacters().getData());
						continue;
					}
				}
				
				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("PositionHistory")) {
						PositionHistory positionHistory = new PositionHistory();
						ArrayList<EmployerOrg> tmp = resume.getEmploymentHistory().getEmployerOrg();
						int len = tmp.size();
						tmp.get(len-1).setPositionHistory(positionHistory);
						event = eventReader.nextEvent();
						continue;
					}
				}
				
				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("Title")) {
						PositionHistory positionHistory = this.getLastPositionHistory(resume);
						event = eventReader.nextEvent();
						positionHistory.setTitle(event.asCharacters().getData());
						this.setLastPositionHistory(resume, positionHistory);
						continue;
					}
				}
				
				if (event.isStartElement()) {
					if (event.asStartElement().getName().getLocalPart()
							.equals("Description")) {
						PositionHistory positionHistory = this.getLastPositionHistory(resume);
						event = eventReader.nextEvent();
						positionHistory.setDescription(event.asCharacters().getData());
						this.setLastPositionHistory(resume, positionHistory);
						continue;
					}
				}
				
				
				
				if (event.asStartElement().getName().getLocalPart()
						.equals(UNIT)) {
					event = eventReader.nextEvent();
					//item.setUnit(event.asCharacters().getData());
					continue;
				}

				if (event.asStartElement().getName().getLocalPart()
						.equals(CURRENT)) {
					event = eventReader.nextEvent();
					//item.setCurrent(event.asCharacters().getData());
					continue;
				}

				if (event.asStartElement().getName().getLocalPart()
						.equals(INTERACTIVE)) {
					event = eventReader.nextEvent();
					//item.setInteractive(event.asCharacters().getData());
					continue;
				}
				// If we reach the end of an item element, we add it to the list
				if (event.isEndElement()) {
					EndElement endElement = event.asEndElement();
					if (endElement.getName().getLocalPart() == (ITEM)) {
						//items.add(item);
					}
				}

			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (XMLStreamException e) {
			e.printStackTrace();
		}
		return items;
	}

} 