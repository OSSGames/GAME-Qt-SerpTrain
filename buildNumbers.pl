#!/usr/bin/perl

$arg = shift;

if ( $arg eq "pre" ) {
    open FROM, "VersionInformation.cpp";
    while (read FROM, $buf, 4096)
    {
	$text = $text . $buf;
    }
    close FROM;

    $md5sum = qx(ls -laR [^V]*.cpp *.h|md5sum);
    chomp($md5sum);
    if (! ($text =~ /Md5Sum\s*=\s*"$md5sum"/) ) {
	if ($text =~ /BuildNumber\s*=\s*Q_INT64_C\((\d*)\)/) {
	    $number = $1 + 1;
            $text =~ s/BuildNumber\s*=\s*Q_INT64_C\((\d*)\)/BuildNumber = Q_INT64_C($number)/;
	    $text =~ s/Md5Sum\s*=\s*"([^"]*)"/Md5Sum = "$md5sum"/;
            print "New buildnumber is $number\n";
	}
        else {
	    print "No buildnumber found!\n";
            system "zenity --error --text='No buildnumber found!'";
	    exit 1;
        }

	open (TO, '>VersionInformation.cpp');
        print TO $text;
	close TO; 
    } else {
	print "No new buildnumber needed!\n";
    }
} elsif ($arg eq "post")  {
    open FROM, "VersionInformation.cpp";
    while (read FROM, $buf, 4096)
    {
	$text = $text . $buf;
    }
    close FROM;

    $md5sum = qx(ls -laR [^V]*.cpp *.h|md5sum);
    chomp($md5sum);
    $text =~ s/Md5Sum\s*=\s*"([^"]*)"/Md5Sum = "$md5sum"/;

    open (TO, '>VersionInformation.cpp');
    print TO $text;
    close TO; 
} else {
    print "Wrong argument!\n";
    system "zenity --error --text='Wrong argument!'";
    exit 1;
}

exit 0;


