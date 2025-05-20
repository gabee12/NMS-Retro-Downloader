#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cstdlib>

using namespace std;

struct ManifestChoice {
    string manifestID;
    bool isCustom;
    int versionIndex; // -1 if custom
};

#ifdef _WIN32 // Define how to handle things depending on OS
	#include <windows.h>
	#define PATH_CHECK_CMD "where"
    const string SDD_EXECUTABLE = "DepotDownloader.exe";

	

	bool runDepotDownloaderWindows(const string& fullCommand) {
    	// Construct mutable buffers
    	string cmd = fullCommand;
    	STARTUPINFOA si = { sizeof(si) };
    	PROCESS_INFORMATION pi;

    	// Create the process
    	if (!CreateProcessA(
        	NULL,            // Application name (use full path if needed)
        	cmd.data(),      // Command line (must be mutable)
        	NULL, NULL,      // Process & thread security attributes
        	FALSE,           // Inherit handles
        	0,               // Creation flags
        	NULL,            // Environment
        	NULL,            // Current directory
        	&si, &pi)){      // Startup and process info
        	cerr << "CreateProcess failed (" << GetLastError() << ")\n";
        	return false;
    	}

    // Wait until child process exits.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Close handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}
#elif __linux__
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>
    #define PATH_CHECK_CMD "which"
    const string SDD_EXECUTABLE = "DepotDownloader";



	bool runDepotDownloaderLinux(const string& executablePath, const vector<string>& args) {
    	pid_t pid = fork();
    	if (pid == -1) {
       	 	perror("fork");
       		return false;
    	}

    	if (pid == 0) {
        	// Child process
        	vector<char*> argv;
        	argv.push_back(const_cast<char*>(executablePath.c_str()));
        	for (const auto& arg : args) {
            	argv.push_back(const_cast<char*>(arg.c_str()));
        	}
        	argv.push_back(nullptr); // NULL-terminate

        	execvp(executablePath.c_str(), argv.data());

        	// If execv returns, it failed
        	perror("execvp");
        	exit(1);
    	} else {
        	// Parent process
        	int status;
        	waitpid(pid, &status, 0);
        	return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    	}
	}
#endif

bool isInPath() { // Execute the command to check if Steam Depot Downloader is in path
	#ifdef _WIN32
		string checkCommand = "where " + SDD_EXECUTABLE + " >nul 2>&1";
	#elif __linux__
		string checkCommand = "which " + SDD_EXECUTABLE + " >/dev/null 2>&1";
	#endif
	return system(checkCommand.c_str()) == 0;
}

string askForPath() { // If unable to Find Steam Depot Downloader automatically, ask user for installation folder
	string userPath;
	cout << "The Steam Depot Downloader executable was not found in your PATH\n";
	cout << "Please enter the full path to the folder containing the executable:\n> ";
	getline(cin, userPath);

	// Cleanup and sanitize
	filesystem::path fullPath(userPath);
	fullPath /= SDD_EXECUTABLE;

	if (!filesystem::exists(fullPath)) {
		cerr << "Executable not found at: " << fullPath << endl;
		exit(1);
	}
	
	return fullPath.string();
}

ManifestChoice getManifestID() { // Release to Manifest ID relation
	vector<pair<string, string>> versions = {
		{"v1.03 (Initial PC release)", "4186508802612155038"},
		{"v1.09 (Last Pre-Foundations Release)", "7324577403707723494"},
		{"v1.10 (Foundations Release)", "5075558467668199049"},
		{"v1.13 (Last Pre-Pathfinder Release)", "2123008115602074603"},
		{"v1.20 (Pathfinder Release)", "55712060249788947"},
		{"v1.24 (Last Pre-Atlas Release)", "3749359456608052294"},
		{"v1.30 (Atlas Rises Release)", "5829957320198344021"},
		{"v1.38 (Last Pre-NEXT Release)", "8262658978126728861"},
		{"v1.50 (NEXT Release)", "5562475608654529864"},
		{"v1.65 (Last Pre-Abyss Release)", "3343700209726312828"},
		{"v1.70 (Abyss Release)", "2832081963478769082"},
		{"v1.71 (Last Pre-Visions Release)", "4290382675093577172"},
		{"v1.75 (Visions Release)", "3343700209726312828"},
		{"v1.77 (Last Pre-Beyond Release)", "7284915656924318617"},
		{"v2.00 (Beyond Release)", "6444776587133164044"},
		{"v2.16 (Last Pre-Synthesis Release)", "3316653034691579082"},
		{"v2.20 (Synthesis Release)", "5557595420772430043"},
		{"v2.27 (Last Pre-Living Release)", "4131722998131020791"},
		{"v2.30 (Living Ship Release)", "6061128983811426614"},
		{"v2.34 (Last Pre-Exo Release)", "99407181682787874"},
		{"v2.40 (Exo Release)", "5037725314024581417"},
		{"v2.42 (Last Pre-Crossplay Release)", "7464613103538631399"},
		{"v2.50 (Crossplay Release)", "2426319044509525725"},
		{"v2.55 (Last Pre-Desolation Release)", "1051177205616279980"},
		{"v2.60 (Desolation Release)", "4344302778791321246"},
		{"v2.62 (Last Pre-Origins Release)", "1941000434979213763"},
		{"v3.00 (Origins Release)", "78369347144021934"},
		{"v3.05 (Last Pre-NextGen Release)", "1852752019883284397"},
		{"v3.10 (NextGen Release)", "5764601304562343181"},
		{"v3.15 (Last Pre-Companions Release)", "7446069109246682091"},
		{"v3.20 (Companions Release)", "9208678582961796774"},
		{"v3.22 (Last Pre-Expeditions Release)", "3671633697766850274"},
		{"v3.30 (Expeditions Release)", "5516940513236609971"},
		{"v3.42 (Last Pre-Prisms Release)", "164905428981274941"},
		{"v3.50 (Prisms Release)", "2774432207931661264"},
		{"v3.53 (Last Pre-Frontiers Release)", "8457564640834371088"},
		{"v3.60 (Frontiers Release)", "5585581420508229976"},
		{"v3.75 (Last Pre-Sentinel Release)", "5184063076020885755"},
		{"v3.80 (Sentinel Release)", "3433632251595884267"},
		{"v3.84 (Last Pre-Outlaws Release)", "6056621687197733627"},
		{"v3.85 (Outlaws Release)", "3687871423133793968"},
		{"v3.93 (Last Pre-Endurance Release)", "7811366465326827399"},
		{"v3.94 (Endurance Release)", "1095687494021103302"},
		{"v3.99.1 (Last Pre-Waypoint Release)", "8802028389294487328"},
		{"v4.00 (Waypoint Release)", "6284068980295993562"},
		{"v4.08 (Last Pre-Fractal Release)", "473296384866151600"},
		{"v4.10 (Fractal Release)", "1014413470066393690"},
		{"v4.15 (Last Pre-Interceptor Release)", "9028934462948511997"},
		{"v4.20 (Interceptor Release)", "6362778589766390941"},
		{"v4.26 (Last Pre-Singularity Release)", "7535669946094198633"},
		{"v4.30 (Singularity Release)", "8241341364757173639"},
		{"v4.38 (Last Pre-Echoes Release)", "8987115219851254482"},
		{"v4.40 (Echoes Release)", "7532610408005154480"},
		{"v4.48 (Last Pre-Omega Release)", "1215178532589006734"},
		{"v4.50 (Omega Release)", "9139355206316637621"},
		{"v4.52 (Last Pre-Orbital Release)", "1353081879671934816"},
		{"v4.60 (Orbital Release)", "5033550799180675126"},
		{"v4.65 (Last Pre-Adrift Release)", "3085037459337735308"},
		{"v4.70 (Adrift Release)", "1109011114410903012"},
		{"v4.72 (Last Pre-Worlds 1 Release)", "4687977719404286055"},
		{"v5.00 (Worlds Part 1 Release)", "634872998300993890"},
		{"v5.05 (Last Pre-Aquarius Release)", "2902362194371977615"},
		{"v5.10 (Aquarius Release)", "3629448325538349905"},
		{"v5.12 (Last Pre-Cursed Release)", "6065565426856499945"},
		{"v5.20 (The Cursed Release)", "2339642079049315971"},
		{"v5.22 (Last Pre-CrossSave Release)", "6429144890906918208"},
		{"v5.25 (CrossSave Release)", "8134167759144738734"},
		{"v5.29 (Last Pre-Worlds 2 Release)", "2627324422079637399"},
		{"v5.50 (Worlds Part 2 Release)", "2904454129872534054"},
		{"v5.58 (Last Pre-Relics Release)", "7253617562963782987"},
		{"v5.60 (Relics Release)", "4860173877924072997"},
		{"v5.63 (Last Release before Current Version)", "2252534952594991828"},
	};

	const int pageSize = 10;
	int totalPages = (versions.size() + pageSize - 1) / pageSize;
	int page = 0;

	while (true) {
		int start = page * pageSize;
		int end = min(start + pageSize, (int)versions.size());

		cout << "\nSelect a version (Page " << (page + 1) << " / " << totalPages << "): \n";
		for (int i = start; i < end; i++) {
			cout << (i + 1) << ". " << versions[i].first << "\n";
		}

		cout << "\nn - Next Page | p - Prev. Page | c - Other Manifest | q - Quit\n> ";
		string input;
		getline(cin, input);
		ManifestChoice choice;

		if (input == "n" && page < totalPages - 1) {
			page++;
		} else if (input == "p" && page > 0) {
			page--;
		} else if (input == "c") {
			cout << "Enter manifest id of desired version:\n> ";
			string custom;
			getline(cin, custom);
			choice.manifestID = custom;
			choice.isCustom = true;
			choice.versionIndex = -1;
			return choice;	
		} else if (input == "q") {
			exit(0);
		} else {
			try {
				int selection = stoi(input);
				if (selection < start + 1 || selection > end) {
					cout << "Invalid selection on this page. Choose an option from " << (start + 1) << " to " << end << ".\n";
					continue;
				}
				if (selection >= 1 && selection <= (int)versions.size()) {
					choice.isCustom = false;
					choice.manifestID = versions[selection-1].second;
					choice.versionIndex = selection - 1;
					return choice;
				} else {
					cout << "Invalid selection. Please enter a valid number.\n";
				}
			} catch (...) {
				cout << "Invalid input. Please enter a number or command.\n";
			}
		}
	}
}

int main() {
	string exePath;

	if (isInPath()) {
		cout << "Steam Depot Downloader found in PATH\n";
		exePath = SDD_EXECUTABLE;
	} else {
		string fullPath = askForPath();
		exePath = fullPath;
	}

	ManifestChoice manifest = getManifestID();

	if (manifest.isCustom || manifest.versionIndex >= 4) {
    cout << "\n[WARNING] Versions v1.20 or above require additional manual steps after download.\n";
    cout << "Please refer to the README in the GitHub repository for detailed instructions.\n\n";
}

	string dir, username;
	cout << "Enter download directory:\n> ";
	filesystem::path dirPath;
	getline(cin, dir);
	dirPath = dir;

	if (!filesystem::exists(dir)) {
		cout << "Directory doesn't exist. Create it? (y/n): ";
		string answer;
		getline(cin, answer);
		if (answer == "y") {
			filesystem::create_directories(dir);
		} else {
			cout << "Exiting...";
			exit(1);
		}
	}

	cout << "Enter Steam username (this is required for Steam Depot Downloader):\n> ";
	getline(cin, username);

	

	

	#ifdef _WIN32
		string command = "\"" + exePath + "\" -app 275850 -depot 275851 -manifest " + manifest.manifestID + " -dir \"" + dirPath.string() + "\" -username \"" + username + "\"";
    	cout << "Starting Steam Depot Downloader:\n" << command << "\n";
		if (!runDepotDownloaderWindows(command)) {
        	cerr << "Failed to execute DepotDownloader via CreateProcess\n";
			return 1;
    	} else {
			cout << "Manifest successfully downloaded. Please refer to GitHub README for instructions on how to launch the game correctly";
			return 0;
		}
	#elif __linux__
		vector<string> args = {
        	"-app", "275850",
        	"-depot", "275851",
        	"-manifest", manifest.manifestID,
        	"-dir", dirPath.string(),
        	"-username", username
    	};

    	if (!runDepotDownloaderLinux(exePath, args)) {
        	cerr << "DepotDownloader execution failed.\n";
			return 1;
    	} else {
			cout << "Manifest successfully downloaded. Please refer to GitHub README for instructions on how to launch the game correctly";
			return 0;
		}
	#endif
}