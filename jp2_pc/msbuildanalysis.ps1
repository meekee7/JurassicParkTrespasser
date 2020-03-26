msbuild .\JP2_PC.sln /p:RunCodeAnalysis=true /v:Detailed /t:Rebuild /p:Configuration=Debug | Tee-Object -File Build\msbuildanalysislog.txt
