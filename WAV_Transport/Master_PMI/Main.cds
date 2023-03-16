<distribution version="17.0.0" name="Main" type="MSI64">
	<prebuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></prebuild>
	<postbuild>
		<workingdir>workspacedir</workingdir>
		<actions></actions></postbuild>
	<msi GUID="{20868A3D-4748-42BA-AFA0-7DD729E82CB6}" x64="true">
		<general appName="Main" outputLocation="f:\CVI_Space\PMI\cvidistkit.Main" relOutputLocation="cvidistkit.Main" outputLocationWithVars="f:\CVI_Space\PMI\cvidistkit.%name" relOutputLocationWithVars="cvidistkit.%name" upgradeBehavior="1" autoIncrement="true" version="1.0.1">
			<arp company="" companyURL="" supportURL="" contact="" phone="" comments=""/>
			<summary title="" subject="" keyWords="" comments="" author=""/></general>
		<userinterface language="Chinese (Simplified)" showPaths="true" showRuntimeOnly="true" readMe="" license="">
			<dlgstrings welcomeTitle="Main" welcomeText=""/></userinterface>
		<dirs appDirID="101">
			<installDir name="[Start&gt;&gt;Programs]" dirID="7" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="[Program Files]" dirID="24" parentID="-1" isMSIDir="true" visible="true" unlock="false"/>
			<installDir name="Main" dirID="100" parentID="7" isMSIDir="false" visible="true" unlock="false"/>
			<installDir name="Main" dirID="101" parentID="24" isMSIDir="false" visible="true" unlock="false"/></dirs>
		<files>
			<simpleFile fileID="0" sourcePath="f:\CVI_Space\PMI\main.uir" relSourcePath="main.uir" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="1" sourcePath="f:\CVI_Space\PMI\cvibuild.main\Release64\main.exe" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="2" sourcePath="f:\CVI_Space\PMI\ui_setting.c" relSourcePath="ui_setting.c" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="3" sourcePath="f:\CVI_Space\PMI\wav_read.c" relSourcePath="wav_read.c" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="4" sourcePath="f:\CVI_Space\PMI\crcLib.c" relSourcePath="crcLib.c" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="5" sourcePath="f:\CVI_Space\PMI\protocol.c" relSourcePath="protocol.c" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="6" sourcePath="f:\CVI_Space\PMI\com_port.c" relSourcePath="com_port.c" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/>
			<simpleFile fileID="7" sourcePath="f:\CVI_Space\PMI\main.c" relSourcePath="main.c" relSourceBase="0" targetDir="101" readonly="false" hidden="false" system="false" regActiveX="false" runAfterInstallStyle="IMMEDIATELY_RESUME_INSTALL" cmdLineArgs="" runAfterInstall="false" uninstCmdLnArgs="" runUninst="false"/></files>
		<fileGroups>
			<projectOutput64 targetType="0" dirID="101" projectID="0">
				<fileID>1</fileID></projectOutput64>
			<projectDependencies64 dirID="101" projectID="0"/></fileGroups>
		<shortcuts>
			<shortcut name="Main" targetFileID="1" destDirID="100" cmdLineArgs="" description="" runStyle="NORMAL"/></shortcuts>
		<mergemodules/>
		<products/>
		<runtimeEngine installToAppDir="false" activeXsup="false" analysis="true" cvirte="true" dotnetsup="true" instrsup="true" lowlevelsup="true" lvrt="true" netvarsup="true" rtutilsup="false">
			<hasSoftDeps/>
			<doNotAutoSelect>
			<component>activeXsup</component>
			<component>rtutilsup</component>
			</doNotAutoSelect></runtimeEngine><sxsRuntimeEngine>
			<selected>false</selected>
			<doNotAutoSelect>false</doNotAutoSelect></sxsRuntimeEngine>
		<advanced mediaSize="650">
			<launchConditions>
				<condition>MINOS_WIN7_SP1</condition>
			</launchConditions>
			<includeConfigProducts>true</includeConfigProducts>
			<maxImportVisible>silent</maxImportVisible>
			<maxImportMode>merge</maxImportMode>
			<custMsgFlag>false</custMsgFlag>
			<custMsgPath>msgrte.txt</custMsgPath>
			<signExe>false</signExe>
			<certificate></certificate>
			<signTimeURL></signTimeURL>
			<signDescURL></signDescURL></advanced>
		<baselineProducts>
			<product name="NI LabWindows/CVI Shared Runtime 2017" UC="{80D3D303-75B9-4607-9312-E5FC68E5BFD2}" productID="{28BFBCB6-5EEE-47F1-A894-FCD877057827}" path="(None)" flavorID="_full_" flavorName="Full" verRestr="false" coreVer="17.0.49157">
				<dependencies>
					<productID>{2D894BCB-6069-418C-BF94-8E5915D8867C}</productID>
					<productID>{2FC99AB8-F11D-4938-B14F-AC60D1016FE7}</productID>
					<productID>{35E63DC2-6BCC-48D6-A1C5-5B068DF34117}</productID>
					<productID>{52981014-740C-430E-A83A-711186DF565B}</productID>
					<productID>{5E99102B-D07A-4D85-8D50-4BE55E3633A0}</productID>
					<productID>{60684600-163F-45D7-83DB-E247FA48D81F}</productID>
					<productID>{64ECB814-3A6A-4E48-9D2F-D6C2EDD725B7}</productID>
					<productID>{69181DFA-D7CA-4AFE-98E2-A985F32B557B}</productID>
					<productID>{6A8A28A0-4568-4718-A3E7-F951C191602B}</productID>
					<productID>{81710CDC-7B4D-4A91-8D44-8E2F8B5C2B6E}</productID>
					<productID>{820FB6E8-8856-48D7-934C-76169DCCFD48}</productID>
					<productID>{88104A0D-1729-4A73-B798-80BEFC243F18}</productID>
					<productID>{88DFF01E-7C87-4E35-B194-CB7A2E32FA6D}</productID>
					<productID>{A4FAF873-938B-43B8-BBF2-E4B2C0CE730B}</productID>
					<productID>{AFC999BB-F270-46EF-B748-AE755EC75322}</productID>
					<productID>{B0049D5A-380E-4219-A69D-02273F86891C}</productID>
					<productID>{B29007A1-E053-46E4-8A51-6803638AF06D}</productID>
					<productID>{BEA1952D-1F00-4336-9A76-C41FA181C02D}</productID>
					<productID>{C0A9F5E2-DCD5-44C1-8B03-C560F4C06D6C}</productID>
					<productID>{E2821E0B-CB08-41BD-AE01-A9068F7B8771}</productID>
					<productID>{ED664E5A-6D28-48EA-A2EE-B6356EF01B37}</productID>
					<productID>{EFC648C5-F3BC-4096-9AFE-23121EF06828}</productID>
					<productID>{F2EEE63A-0DCA-41BF-A243-4E4C0DFA38A4}</productID>
					<productID>{F8018104-6294-4896-A9E2-CBC919992310}</productID>
					<productID>{FA85DBC5-063E-4CA0-9E3A-7C1180D8F7AE}</productID>
					<productID>{FAFB3CD7-F8A6-4D31-BCB4-57354C17128E}</productID>
					<productID>{FF35E2A2-9214-4B76-91AE-5567076AAE8C}</productID></dependencies></product></baselineProducts>
		<Projects NumProjects="1">
			<Project000 ProjectID="0" ProjectAbsolutePath="f:\CVI_Space\PMI\main.prj" ProjectRelativePath="main.prj"/></Projects>
		<buildData progressBarRate="0.676331385808927">
			<progressTimes>
				<Begin>0.000000000000000</Begin>
				<ProductsAdded>0.057763500000000</ProductsAdded>
				<DPConfigured>1.232755500000000</DPConfigured>
				<DPMergeModulesAdded>3.377757000000000</DPMergeModulesAdded>
				<DPClosed>11.402764500000004</DPClosed>
				<DistributionsCopied>16.177204000000003</DistributionsCopied>
				<End>147.856512499999980</End></progressTimes></buildData>
	</msi>
</distribution>
