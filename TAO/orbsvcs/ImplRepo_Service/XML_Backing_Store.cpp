// $Id$

#include "XML_Backing_Store.h"
#include "Server_Info.h"
#include "Activator_Info.h"
#include "utils.h"
#include "Locator_XMLHandler.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ACEXML/parser/parser/Parser.h"
#include "ACEXML/common/FileCharStream.h"
#include "ACEXML/common/XML_Util.h"

XML_Backing_Store::XML_Backing_Store(const Options& opts,
                                     CORBA::ORB_ptr orb,
                                     bool suppress_erase)
: Locator_Repository(opts, orb),
  filename_(opts.persist_file_name())
{
  if (opts.repository_erase() && !suppress_erase)
    {
      ACE_OS::unlink ( this->filename_.c_str () );
    }
}

XML_Backing_Store::~XML_Backing_Store()
{
}

int
XML_Backing_Store::persistent_remove (const ACE_CString& , bool )
{
  // one big XML file, need to persist everything
  return persist();
}

int
XML_Backing_Store::persistent_update(const Server_Info_Ptr& , bool )
{
  // one big XML file, need to persist everything
  return persist();
}

int
XML_Backing_Store::persistent_update(const Activator_Info_Ptr& , bool )
{
  // one big XML file, need to persist everything
  return persist();
}

int
XML_Backing_Store::persist ()
{
  FILE* fp = ACE_OS::fopen (this->filename_.c_str (), "w");
  if (fp == 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Couldn't write to file %C\n"),
        this->filename_.c_str()));
      return -1;
    }
  ACE_OS::fprintf (fp,"<?xml version=\"1.0\"?>\n");
  ACE_OS::fprintf (fp,"<%s>\n", Locator_XMLHandler::ROOT_TAG);

  // Save servers
  Locator_Repository::SIMap::ENTRY* sientry = 0;
  Locator_Repository::SIMap::ITERATOR siit (this->servers ());
  for (; siit.next (sientry); siit.advance() )
    {
      persist(fp, *sientry->int_id_, "\t");
    }

  // Save Activators
  Locator_Repository::AIMap::ENTRY* aientry = 0;
  Locator_Repository::AIMap::ITERATOR aiit (this->activators ());
  for (; aiit.next (aientry); aiit.advance ())
    {
      ACE_CString aname = aientry->ext_id_;
      persist(fp, *aientry->int_id_, "\t");
    }

  ACE_OS::fprintf (fp,"</%s>\n", Locator_XMLHandler::ROOT_TAG);
  ACE_OS::fclose (fp);

  return 0;
}

void
XML_Backing_Store::persist (FILE* fp, const Server_Info& info,
                            const char* tag_prepend)
{
  ACE_CString server_id = ACEXML_escape_string (info.server_id);
  ACE_CString name = ACEXML_escape_string (info.name);
  ACE_CString activator = ACEXML_escape_string (info.activator);
  ACE_CString cmdline = ACEXML_escape_string (info.cmdline);
  ACE_CString wdir = ACEXML_escape_string (info.dir);
  ACE_CString partial_ior = ACEXML_escape_string (info.partial_ior);
  ACE_CString ior = ACEXML_escape_string (info.ior);

  ACE_OS::fprintf (fp,"%s<%s", tag_prepend,
    Locator_XMLHandler::SERVER_INFO_TAG);
  ACE_OS::fprintf (fp," server_id=\"%s\"", server_id.c_str ());
  ACE_OS::fprintf (fp," name=\"%s\"", name.c_str ());
  ACE_OS::fprintf (fp," activator=\"%s\"", activator.c_str ());
  ACE_OS::fprintf (fp," command_line=\"%s\"", cmdline.c_str ());
  ACE_OS::fprintf (fp," working_dir=\"%s\"", wdir.c_str ());
  ACE_CString amodestr =
    ImR_Utils::activationModeToString (info.activation_mode);
  ACE_OS::fprintf (fp," activation_mode=\"%s\"", amodestr.c_str ());
  ACE_OS::fprintf (fp," start_limit=\"%d\"", info.start_limit);
  ACE_OS::fprintf (fp," partial_ior=\"%s\"", partial_ior.c_str ());
  ACE_OS::fprintf (fp," ior=\"%s\"", ior.c_str ());

  const CORBA::ULong length = info.env_vars.length ();
  if (length > 0)
    {
      ACE_OS::fprintf (fp,">\n");
      for (CORBA::ULong i = 0; i < info.env_vars.length (); ++i)
        {
          ACE_OS::fprintf (fp,"%s\t<%s", tag_prepend,
            Locator_XMLHandler::ENVIRONMENT_TAG);
          ACE_OS::fprintf (fp," name=\"%s\"", info.env_vars[i].name.in ());
          ACE_CString val = ACEXML_escape_string(info.env_vars[i].value.in());
          ACE_OS::fprintf (fp," value=\"%s\"", val.c_str());
          ACE_OS::fprintf (fp,"/>\n");
        }

      ACE_OS::fprintf (fp,"%s</%s>\n", tag_prepend,
        Locator_XMLHandler::SERVER_INFO_TAG);
    }
  else
    {
      ACE_OS::fprintf (fp,"/>\n");
    }
}

void
XML_Backing_Store::persist (FILE* fp, const Activator_Info& info,
                            const char* tag_prepend)
{
  ACE_OS::fprintf (fp,"%s<%s", tag_prepend,
    Locator_XMLHandler::ACTIVATOR_INFO_TAG);
  ACE_OS::fprintf( fp," name=\"%s\"", info.name.c_str ());
  ACE_OS::fprintf (fp," token=\"%d\"", info.token);
  ACE_OS::fprintf (fp," ior=\"%s\"", info.ior.c_str ());
  ACE_OS::fprintf (fp,"/>\n");
}

int
XML_Backing_Store::init_repo(PortableServer::POA_ptr )
{
  // ignore load return since files don't have to exist
  load(this->filename_);
  return 0;
}

int
XML_Backing_Store::load (const ACE_TString& filename)
{
  Locator_XMLHandler xml_handler (*this);
  return load(filename, xml_handler, this->opts_.debug());
}

int
XML_Backing_Store::load (const ACE_TString& filename,
                         ACEXML_DefaultHandler& xml_handler,
                         unsigned int debug)
{
  // xml input source will take ownership
  ACEXML_FileCharStream* fstm = new ACEXML_FileCharStream;

  const int err = fstm->open (filename.c_str());
  if (debug > 9)
    {
      ACE_DEBUG((LM_INFO, ACE_TEXT ("load %s%s\n"), filename.c_str(),
        ((err == 0) ? "" : " (file doesn't exist)")));
    }

  if (err != 0)
    {
      delete fstm;
      return err;
    }

  ACEXML_Parser parser;

  // InputSource takes ownership
  ACEXML_InputSource input (fstm);

  parser.setContentHandler (&xml_handler);
  parser.setDTDHandler (&xml_handler);
  parser.setErrorHandler (&xml_handler);
  parser.setEntityResolver (&xml_handler);

  try
    {
      parser.parse (&input);
    }
  catch (const ACEXML_Exception& ex)
    {
      ACE_ERROR ((LM_ERROR,
        ACE_TEXT ("Error during load of ImR persistence xml file (%s)."),
        filename.c_str()));
      ex.print ();
      return -1;
    }

  return 0;
}

const char*
XML_Backing_Store::repo_mode() const
{
  return this->filename_.c_str();
}