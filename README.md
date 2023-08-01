# RoomBasedMapGeneratorUE

[HacknPlan Project](https://app.hacknplan.com/p/182193/dashboards/project)

## Setup

1. Clone repo.
2. Add below line to "post-checkout", "post-commit", and "post-merge" Git hooks in ".git\hooks":
```
"Plugins/AutoVersioning/VersioningTool.exe Git $(git rev-parse --show-toplevel) PreCommitMode MergeUEVersion ApplyTagPush ApplyUEProjectVersion"
```
3. Install Epic Games launcher.
4. Open Epic Games Launcher and go to the "Unreal Engine" tab on the left.
    - **Install Latest UE Version:** On the top right, click the "Install Engine" button then "Install".
    - **Install Specific UE Version:** Go to the "Library" tab on the top. Click the "+" button by the "Engine Versions" header. Using the version dropdown on the generated engine card in the "Engine Versions" section, select the desired UE version and click "Install".
5. Install Visual Studio.
6. Right click ".uproject" file at the root of the projects directory and select "Generate Visual Studio project files"
7. Open generated ".sln" file in Visual Studio and build.
8. Open project in UE.
9. On the bottom right, click "Revision Control"->"Connect Revision Control". In the "Provider" dropdown, select "Git LFS 2". Check Git Path value is correct and "Uses Git LFS" is checked. Make sure username matches your Git username. Click "Accept Settings"