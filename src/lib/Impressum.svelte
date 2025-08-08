<script lang="ts">
	import TextBox from './TextBox.svelte';

	function close() {
		console.log('close');
		show = false;
	}

	interface Props {
		show?: boolean;
	}

	let { show = $bindable(false) }: Props = $props();
</script>

<TextBox title="Impressum" {show} {close}>
	<h2>Kontakt</h2>
	<address>
		Prof. Dr. Dirk W. Hoffmann<br />
		Hochschule Karlsruhe<br />
		Moltkestraße 30<br />
		D - 76133 Karlsruhe<br />
		Germany
	</address>
	<h2>Datenschutz</h2>
	Unser Hoster erhebt in sog. Logfiles folgende Daten, die Ihr Browser übermittelt: IP-Adresse,
	die Adresse der vorher besuchten Website (Referer Anfrage-Header), Datum und Uhrzeit der Anfrage, Zeitzonendifferenz
	zur Greenwich Mean Time, Inhalt der Anforderung, HTTP-Statuscode, übertragene Datenmenge, Website,
	von der die Anforderung kommt und Informationen zu Browser und Betriebssystem. Das ist erforderlich,
	um unsere Website anzuzeigen und die Stabilität und Sicherheit zu gewährleisten. Dies entspricht unserem
	berechtigten Interesse im Sinne des Art. 6 Abs. 1 S. 1 lit. f DSGVO. Es erfolgt kein Tracking und wir
	haben auf diese Daten keinen direkten Zugriff. Der Inhalt der auf dieser Webseite eingelesenen Datenträgerabbilder 
	werden ausschließlich lokal im Browser gespeichert. Es gelangen keine Daten aus den Datenträgerabbildern auf den 
	Server und es werden keine statistischen Daten erfasst.
	
	Wir setzen für die Zurverfügungstellung unserer Website folgenden Hoster ein: GitHub Inc. 88 Colin P Kelly Jr St San Francisco, CA 94107 United States Dieser
	ist Empfänger Ihrer personenbezogenen Daten. Dies entspricht unserem berechtigten Interesse im Sinne
	des Art. 6 Abs. 1 S. 1 lit. f DSGVO, selbst keinen Server in unseren Räumlichkeiten vorhalten zu müssen.
	Serverstandort ist USA. Weitere Informationen zu Widerspruchs- und Beseitigungsmöglichkeiten gegenüber
	GitHub finden Sie unter: https://docs.github.com/en/free-pro-team@latest/github/site-policy/github-privacy-statement#github-pages
	Sie haben das Recht der Verarbeitung zu widersprechen. Ob der Widerspruch erfolgreich ist, ist im Rahmen
	einer Interessenabwägung zu ermitteln. Die Daten werden gelöscht, sobald der Zweck der Verarbeitung
	entfällt. Die Verarbeitung der unter diesem Abschnitt angegebenen Daten ist weder gesetzlich noch vertraglich
	vorgeschrieben. Die Funktionsfähigkeit der Website ist ohne die Verarbeitung nicht gewährleistet. GitHub
	hat Compliance-Maßnahmen für internationale Datenübermittlungen umgesetzt. Diese gelten für alle weltweiten
	Aktivitäten, bei denen GitHub personenbezogene Daten von natürlichen Personen in der EU verarbeitet.
	Diese Maßnahmen basieren auf den EU-Standardvertragsklauseln (SCCs). Weitere Informationen finden Sie
	unter: https://docs.github.com/en/free-pro-team@latest/github/site-policy/github-data-protection-addendum#attachment-1–the-standard-contractual-clauses-processors

	<!--
	<h2>Datenschutz</h2>
	<p>
		Durch die Nutzung unserer Website erklären Sie sich mit der Erhebung, Verarbeitung und Nutzung
		von Daten gemäß der nachfolgenden Beschreibung einverstanden. Unsere Website kann grundsätzlich
		ohne Registrierung besucht werden. Dabei werden Daten wie beispielsweise aufgerufene Seiten bzw.
		Namen der abgerufenen Datei, Datum und Uhrzeit zu statistischen Zwecken auf dem Server
		gespeichert, ohne dass diese Daten unmittelbar auf Ihre Person bezogen werden. Personenbezogene
		Daten, insbesondere Name, Adresse oder E-Mail-Adresse werden soweit möglich auf freiwilliger
		Basis erhoben. Ohne Ihre Einwilligung erfolgt keine Weitergabe der Daten an Dritte.
	</p>
	<h2>Hosting</h2>
	<p>
		Diese Webseite wird gehostet auf Servern der Github Inc., 88 Colin P Kelly Jr. Street, San
		Francisco, CA 94107, USA. GitHub kann beim Aufruf dieser Webseiten technische Informationen
		inklusive ihrer IP Adresse aufzeichnen. Weitere Informationen zum Umgang mit Nutzerdaten finden
		Sie in der
		<a href="https://help.github.com/articles/github-privacy-policy"
			>Datenschutzerklärung von GitHub</a
		>.
	</p>
	-->
</TextBox>
