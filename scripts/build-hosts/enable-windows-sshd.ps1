# Enable OpenSSH Server on a Windows build host and authorize one public key.
# Run in an elevated PowerShell (Run as Administrator), as the account that
# will be used for SSH logins:
#   Set-ExecutionPolicy -Scope Process Bypass
#   .\enable-windows-sshd.ps1 -PublicKey 'ssh-ed25519 AAAA... name'
# Safe to re-run.
param([Parameter(Mandatory)][string]$PublicKey)
$ErrorActionPreference = 'Stop'

$principal = [Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    throw 'Run this from an elevated PowerShell (Run as Administrator).'
}

# 1. Install the OpenSSH Server capability.
$cap = Get-WindowsCapability -Online -Name 'OpenSSH.Server*' | Select-Object -First 1
if ($cap.State -ne 'Installed') {
    Write-Host "Installing $($cap.Name) ..."
    Add-WindowsCapability -Online -Name $cap.Name | Out-Null
}

# 2. Start sshd now and at boot (first start generates host keys and sshd_config).
Set-Service -Name sshd -StartupType Automatic
Start-Service sshd

# 3. Firewall: TCP 22 inbound, local subnet only, any network profile.
$ruleName = 'OpenSSH-Server-In-TCP'
if (-not (Get-NetFirewallRule -Name $ruleName -ErrorAction SilentlyContinue)) {
    New-NetFirewallRule -Name $ruleName -DisplayName 'OpenSSH Server (sshd)' `
        -Direction Inbound -Protocol TCP -LocalPort 22 -Action Allow | Out-Null
}
Set-NetFirewallRule -Name $ruleName -Enabled True -Profile Any -RemoteAddress LocalSubnet

# 4. Authorize the key. Members of Administrators read
#    C:\ProgramData\ssh\administrators_authorized_keys (sshd ignores ~\.ssh for them);
#    other users read ~\.ssh\authorized_keys. Write both.
function Add-Key([string]$Path, [string[]]$GrantArgs) {
    $dir = Split-Path $Path
    if (-not (Test-Path $dir)) { New-Item -ItemType Directory -Path $dir | Out-Null }
    $existing = if (Test-Path $Path) { @(Get-Content $Path) } else { @() }
    if ($existing -notcontains $PublicKey) {
        Set-Content -Path $Path -Value ($existing + $PublicKey) -Encoding ascii
    }
    icacls $Path /inheritance:r @GrantArgs | Out-Null
}
$me = "$env:USERDOMAIN\$env:USERNAME"
Add-Key "$env:ProgramData\ssh\administrators_authorized_keys" @('/grant', '*S-1-5-32-544:F', '/grant', '*S-1-5-18:F')
Add-Key "$env:USERPROFILE\.ssh\authorized_keys" @('/grant', "${me}:F", '/grant', '*S-1-5-18:F')

# 5. Default login shell: Windows PowerShell (instead of cmd.exe).
if (-not (Test-Path 'HKLM:\SOFTWARE\OpenSSH')) { New-Item -Path 'HKLM:\SOFTWARE\OpenSSH' | Out-Null }
New-ItemProperty -Path 'HKLM:\SOFTWARE\OpenSSH' -Name DefaultShell -PropertyType String -Force `
    -Value "$env:SystemRoot\System32\WindowsPowerShell\v1.0\powershell.exe" | Out-Null

# 6. Key-only logins.
$cfg = "$env:ProgramData\ssh\sshd_config"
(Get-Content $cfg) -replace '^#?\s*PasswordAuthentication\s+\S+', 'PasswordAuthentication no' |
    Set-Content -Path $cfg -Encoding ascii
Restart-Service sshd

# 7. Report what the client needs.
Write-Host ''
Write-Host "SSH user:  $env:USERNAME"
Write-Host "Hostname:  $env:COMPUTERNAME"
Get-NetIPAddress -AddressFamily IPv4 -PrefixOrigin Dhcp, Manual -ErrorAction SilentlyContinue |
    ForEach-Object { Write-Host "IPv4:      $($_.IPAddress) ($($_.InterfaceAlias))" }
Write-Host 'Host key fingerprint (compare on first connect):'
& "$env:SystemRoot\System32\OpenSSH\ssh-keygen.exe" -lf "$env:ProgramData\ssh\ssh_host_ed25519_key.pub"
